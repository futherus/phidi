#pragma once

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

template<typename T,
         bool IsVertical>
class Flex final
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
    Flex( LayoutPolicy layout_policy)
        : layout_policy_{ layout_policy}
        , widgets_{}
    {}

    Flex( const Flex&) = delete;
    Flex& operator=( const Flex&) = delete;
    Flex( Flex&&) = delete;
    Flex& operator=( Flex&&) = delete;
    ~Flex() = default;

public:
    LayoutPolicy getLayoutPolicy() const { return layout_policy_; }

private:
    LayoutPolicy layout_policy_;
    container widgets_;
};

template<typename T,
         bool IsVertical>
inline void
Render( const Flex<T, IsVertical>&,
        const Geometry&,
        sf::RenderTarget&)
{}

// FIXME: MainAxisAlignment is not used.
template<typename T,
         bool IsVertical>
inline LayoutObject
Layout( const Flex<T, IsVertical>& flex,
        const Constraints& cons)
{$FUNC
    float total_flex = 0;
    float cross_size = 100;
    float main_space = IsVertical ? cons.height() : cons.width();
    std::vector<LayoutObject> fixed;

    assert( flex.size() > 0 && "Flex is empty");

    for ( auto& pair : flex)
    {
        if ( pair.second == 0 )
        {
            Constraints child_cons = IsVertical
                                     ? Constraints{ cons.width(), main_space}
                                     : Constraints{ main_space, cons.height()};

            LayoutObject child = Layout( pair.first, child_cons);
            cross_size = std::max( cross_size, IsVertical
                                               ? child.getSize().x
                                               : child.getSize().y);

            main_space -= IsVertical
                          ? child.getSize().y
                          : child.getSize().x;

            fixed.push_back( std::move( child));
        } else
        {
            total_flex += pair.second;
        }
    }

    float flex_weight = (total_flex > 0) ? main_space / total_flex : 0;
    LayoutObject self{ flex, flex.size()};
    size_t fixed_indx = 0;

    for ( auto& pair : flex)
    {
        if ( pair.second == 0 )
        {
            self.push_back( std::move( fixed.at( fixed_indx)));
            fixed_indx++;
        } else
        {
            float s = pair.second * flex_weight;
            Constraints child_cons = IsVertical
                                     ? Constraints{ cons.width(), s}
                                     : Constraints{ s, cons.height()};

            LayoutObject child = Layout( pair.first, child_cons);
            $D( "Flex<T, %s> child size: (%f, %f)\n", IsVertical ? "True" : "False", child.getSize().x, child.getSize().y);
            assert( s == (IsVertical ? child.getSize().y : child.getSize().x) && "Flex child must occupy all provided space");
            cross_size = std::max( cross_size, IsVertical
                                               ? child.getSize().x
                                               : child.getSize().y);

            self.push_back( std::move( child));
        }
    }

    float curr = 0;

    for ( auto& child : self )
    {
        float cross_pos = 0;
        float main_child_size  = IsVertical
                                 ? child.getSize().y
                                 : child.getSize().x;

        float cross_child_size = IsVertical
                                 ? child.getSize().x
                                 : child.getSize().y;

        switch ( flex.getLayoutPolicy().cross_align_)
        {
            case CrossAxisAlignment::Start:
                cross_pos = 0;
                break;
            case CrossAxisAlignment::End:
                cross_pos = cross_size - cross_child_size;
                break;
            case CrossAxisAlignment::Center:
                cross_pos = (cross_size - cross_child_size) / 2;
                break;
            default:
                assert( 0);
                break;
        }
        sf::Vector2f pos = IsVertical
                           ? sf::Vector2f{ cross_pos, curr}
                           : sf::Vector2f{ curr, cross_pos};

        child.setPosition( pos);
        curr += main_child_size;
    }

    sf::Vector2f size = IsVertical
                        ? sf::Vector2f{ cross_size, cons.height()}
                        : sf::Vector2f{ cons.width(), cross_size};

    self.setSize( size);
    $M( "Returning Flex<T, %s> (%f, %f) (%f, %f)\n", IsVertical ? "True" : "False",
                                                     self.getPosition().x, self.getPosition().y, self.getSize().x, self.getSize().y);

    return self;
}

} // namespace xui
