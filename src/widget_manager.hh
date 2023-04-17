#pragma once

#include <vector>
#include <memory>
#include "widget.hh"

namespace xui
{

class WidgetManager
{
public:
    WidgetManager( sf::Vector2f size)
        : size_{ size}
    {}

    WidgetManager( const WidgetManager&) = delete;
    WidgetManager& operator=( const WidgetManager&) = delete;
    WidgetManager( WidgetManager&&) = delete;
    WidgetManager& operator=( WidgetManager&&) = delete;
    ~WidgetManager() = default;

    sf::Vector2f getSize() const { return size_; }
    const std::vector<LayoutDelegate>& getWidgets() const { return widgets_; }
          std::vector<LayoutDelegate>& getWidgets()       { return widgets_; }

private:
    sf::Vector2f size_;
    std::vector<LayoutDelegate> widgets_;
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
    LayoutObject object{ manager, manager.getWidgets().size()};
    object.setSize( cons);

    for ( auto& widget : manager.getWidgets() )
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
