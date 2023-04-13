#pragma once

#include <vector>
#include <memory>
#include "widget.hh"

namespace xui
{

struct WidgetManagerImpl
{
    WidgetManagerImpl( sf::Vector2f size)
        : size_{ size}
    {}

    sf::Vector2f size_;
    std::vector<Widget> widgets_;
};

class WidgetManager
    : public Impl<WidgetManagerImpl>
{
public:
    using Impl<WidgetManagerImpl>::Impl;

    sf::Vector2f getSize() const { return impl().size_; }
    const std::vector<Widget>& getWidgets() const { return impl().widgets_; }
    std::vector<Widget>& getWidgets() { return impl().widgets_; }
};

inline void
Render( WidgetManager,
        const Geometry&,
        sf::RenderTarget&)
{}

inline LayoutObject
Layout( WidgetManager manager,
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
