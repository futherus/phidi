#ifndef WIDGET_HH
#define WIDGET_HH

#include <SFML/Graphics.hpp>
#include "rectangle.hh"
#include "debug.hh"

namespace xui
{

class IWidget
{
private:
    Rectangle bounds_;

    sf::RenderTexture pixels_;
    sf::Sprite sprite_;

    void adjustSprite();

public:
    IWidget( const Rectangle& bounds);

    IWidget( const IWidget&) = delete;
    IWidget( IWidget&&) = delete;
    IWidget& operator=( const IWidget&) = delete;
    IWidget& operator=( IWidget&&) = delete;
    virtual ~IWidget() = default;

    Rectangle bounds() const { return bounds_; }
    bool contains( sf::Vector2f pos) { return bounds_.contains( pos); }

    virtual void onMousePressed( const sf::Event&) {}
    virtual void onMouseReleased( const sf::Event&) {}
    virtual void onMouseMoved( const sf::Event&) {}
    virtual void onKeyPressed( const sf::Event&) {}
    virtual void onKeyReleased( const sf::Event&) {}
    virtual void onTextEntered( const sf::Event&) {}

    virtual void draw( sf::RenderTarget& target) const = 0;
};

} // namespace xui

#endif // WIDGET_HH
