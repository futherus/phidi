#pragma once

#include <vector>
#include <memory>
#include "widgets/widget.hh"

namespace xui
{

class WidgetManager
{
public:
    using value_type = LayoutDelegate;
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
    WidgetManager( sf::Vector2f size)
        : size_{ size}
    {}

    WidgetManager( const WidgetManager&) = delete;
    WidgetManager& operator=( const WidgetManager&) = delete;
    WidgetManager( WidgetManager&&) = delete;
    WidgetManager& operator=( WidgetManager&&) = delete;
    ~WidgetManager() = default;

public:
    sf::Vector2f getSize() const { return size_; }

private:
    sf::Vector2f size_;
    container widgets_;
};

inline void
Render( const WidgetManager&,
        const Geometry&,
        sf::RenderTarget&)
{}

inline LayoutObject
Layout( const WidgetManager& manager,
        const Constraints& cons)
{$FUNC
    LayoutObject object{ manager, manager.size()};
    object.setSize( cons);

    for ( auto& widget : manager )
    {
        LayoutObject child = Layout( widget, cons);

        assert( child.getSize() <= cons);
        object.push_back( std::move( child));
    }

    $M( "returning WidgetManager (%f, %f) (%f, %f)\n", object.getPosition().x, object.getPosition().y, object.getSize().x, object.getSize().y);
    return object;
}

void OnMousePressed(  WidgetManager& manager, const sf::Event& event);
void OnMouseReleased( WidgetManager& manager, const sf::Event& event);
void OnMouseMoved(    WidgetManager& manager, const sf::Event& event);
void OnKeyPressed(    WidgetManager& manager, const sf::Event& event);
void OnKeyReleased(   WidgetManager& manager, const sf::Event& event);
void OnTextEntered(   WidgetManager& manager, const sf::Event& event);

} // namespace xui
