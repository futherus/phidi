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
    using value_type = T;
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

    void push_back( value_type&& widget) { widgets_.push_back( std::move( widget)); }

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

template<typename T>
inline LayoutObject
Layout( const Column<T>& column,
        const Constraints& cons)
{$FUNC
    size_t n_widgets = column.size();
    LayoutObject object{ column, n_widgets};

    const float padding = column.getPadding();

    float curr = 0;
    float vspace = cons.height() - n_widgets * padding;
    float xspace = cons.width() - 2 * padding;
    float col_width = 100;

    for ( const T& widget : column )
    {
        LayoutObject child = Layout( widget, Constraints{ xspace, vspace});

        child.setPosition({ 0, curr});

        vspace -= child.getSize().y;
        curr   += child.getSize().y;

        col_width = std::max( col_width, child.getSize().x);

        object.push_back( std::move( child));
    }

    const float step = (vspace + n_widgets * padding) / (object.size() + 1);
    int i = 1;

    for ( auto& child : object )
    {
        sf::Vector2f pos = child.getPosition();

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
        pos.x += padding;

        pos.y += step * i;
        i++;

        child.setPosition( pos);
    }
    col_width += 2 * padding;

    object.setSize( sf::Vector2f{ col_width, cons.height()});
    $M( "Returning Column<T> (%f, %f) (%f, %f)\n", object.getPosition().x, object.getPosition().y, object.getSize().x, object.getSize().y);
    return object;
}

} // namespace xui
