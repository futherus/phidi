#include "widget.hh"

namespace xui
{

void IWidget::adjustSprite()
{
    // FIXME: temporary, I hope
    sprite_.setPosition(bounds_.tl() - sf::Vector2f{1, 1});
}

IWidget::IWidget( const Rectangle& bounds)
    : bounds_{ bounds}
{
    // FIXME: temporary, I hope
    pixels_.create( (uint32_t) bounds_.width() + 2, (uint32_t) bounds_.height() + 2);
    sprite_.setTexture( pixels_.getTexture());
    adjustSprite();

    sf::Vertex bounding_box[5] = {
        sf::Vertex( {1                  , 0                   }, sf::Color::Red),
        sf::Vertex( {bounds_.width() + 2, 0                   }, sf::Color::Red),
        sf::Vertex( {bounds_.width() + 2, bounds_.height() + 1}, sf::Color::Red),
        sf::Vertex( {1                  , bounds_.height() + 1}, sf::Color::Red),
        sf::Vertex( {1                  , 0                   }, sf::Color::Red)
    };

    pixels_.draw( bounding_box, 5, sf::LineStrip);
    pixels_.display();
}

void IWidget::draw(sf::RenderTarget& target) const
{
    target.draw(sprite_);
}

} // namespace xui