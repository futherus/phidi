#pragma once

#include "widgets/widget.hh"

namespace xui
{

enum class CrossAxisAlignment
{
    Start,
    End,
    Center
};

enum class MainAxisAlignment
{
    Start,
    End,
    Center
};

class LayoutPolicy
{
public:
    MainAxisAlignment main_align_;
    CrossAxisAlignment cross_align_;
};

template<typename T>
class Column final
{
public:
    using value_type = std::pair<T, float>;
    using size_type = size_t;
    using container = std::vector<value_type>;
    using iterator = typename container::iterator;
    using const_iterator = typename container::const_iterator;

    iterator          begin()                   { return widgets_.begin(); }
    const_iterator    begin()             const { return widgets_.begin(); }
    iterator            end()                   { return widgets_.end();   }
    const_iterator      end()             const { return widgets_.end();   }

    size_type          size()             const { return widgets_.size();  }
    bool              empty()             const { return widgets_.empty(); }
          value_type& front()                   { return widgets_.front(); }
    const value_type& front()             const { return widgets_.front(); }
          value_type&  back()                   { return widgets_.back();  }
    const value_type&  back()             const { return widgets_.back();  }
          value_type&    at( size_type i)       { return widgets_.at( i);  }
    const value_type&    at( size_type i) const { return widgets_.at( i);  }

    void
    push_back( T&& widget,
               float flex)
    {
        assert( flex >= 0.0f);
        widgets_.push_back( {std::move( widget), flex});
    }

public:
    Column( float padding,
            LayoutPolicy layout_policy)
        : padding_{ padding}
        , layout_policy_{ layout_policy}
        , widgets_{}
    {}

    Column( const Column&) = delete;
    Column& operator=( const Column&) = delete;
    Column( Column&&) = delete;
    Column& operator=( Column&&) = delete;
    ~Column() = default;

public:
    float getPadding() const { return padding_; }
    LayoutPolicy getLayoutPolicy() const { return layout_policy_; }

private:
    float padding_;
    LayoutPolicy layout_policy_;
    container widgets_;
};

template<typename T>
inline void
Render( const Column<T>&,
        const Geometry&,
        sf::RenderTarget&)
{}

// FIXME: MainAxisAlignment is not used.
template<typename T>
inline LayoutObject
Layout( const Column<T>& column,
        const Constraints& cons)
{$FUNC
    float total_flex = 0;
    float vspace = cons.height();
    float col_width = 100;
    std::vector<LayoutObject> fixed;

    for ( auto& pair : column)
    {
        if ( pair.second == 0 )
        {
            LayoutObject child = Layout( pair.first, Constraints{ cons.width(), vspace});
            col_width = std::max( col_width, child.getSize().x);
            vspace -= child.getSize().y;

            fixed.push_back( std::move( child));
        } else
        {
            total_flex += pair.second;
        }
    }

    float flex_weight = (total_flex > 0) ? vspace / total_flex : 0;
    LayoutObject self{ column, column.size()};
    size_t fixed_indx = 0;

    for ( auto& pair : column)
    {
        if ( pair.second == 0 )
        {
            self.push_back( std::move( fixed.at( fixed_indx)));
            fixed_indx++;
        } else
        {
            LayoutObject child = Layout( pair.first, Constraints{ cons.width(), pair.second * flex_weight});
            assert( child.getSize().y == pair.second * flex_weight && "Child must occupy all provided height");
            col_width = std::max( col_width, child.getSize().x);

            self.push_back( std::move( child));
        }
    }

    float curr = 0;

    for ( auto& child : self )
    {
        sf::Vector2f pos;

        switch ( column.getLayoutPolicy().cross_align_)
        {
            case CrossAxisAlignment::Start:
                pos.x = 0;
                break;
            case CrossAxisAlignment::End:
                pos.x = col_width - child.getSize().x;
                break;
            case CrossAxisAlignment::Center:
                pos.x = (col_width - child.getSize().x) / 2;
                break;
            default:
                assert( 0);
                break;
        }
        pos.y = curr;
        curr += child.getSize().y;

        child.setPosition( pos);
    }

    self.setSize( {col_width, cons.height()});
    $M( "Returning Column<T> (%f, %f) (%f, %f)\n", self.getPosition().x, self.getPosition().y, self.getSize().x, self.getSize().y);

    return self;
}

} // namespace xui
