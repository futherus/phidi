#include "core/debug.hh"
#include "widgets/button.hh"

namespace xui
{

// void
// onMouseMoved( PushButton& button,
//                    const sf::Event& event)
// {
//     float x = static_cast<float>( event.mouseMove.x);
//     float y = static_cast<float>( event.mouseMove.y);

//     // ???????????????????????????????????????? //
//     button.is_hovered_ = button.contains( {x, y});
// }

void
Render( const PushButton& button,
        const Geometry& geometry,
        sf::RenderTarget& target)
{
    const sf::Texture* texture = button.getTextures().at( button.isHovered()).at( button.isPushed());
    assert( texture);

    sf::Sprite sprite;
    sprite.setTexture( *texture, true);
    sprite.setTextureRect( sf::IntRect{ 0, 0, geometry.width(),
                                              geometry.height() });
    sprite.setPosition( geometry.tl());

    target.draw( sprite);
}

LayoutObject
Layout( const PushButton& button,
        const Constraints& cons)
{$FUNC
    // $M( "Layout: Pointer to button: %p\n", button.address());
    LayoutObject object = LayoutObject{ button, Geometry{ {}, cons}};
    $M( "returning PushButton (%f, %f) (%f, %f)\n", object.getPosition().x, object.getPosition().y, object.getSize().x, object.getSize().y);
    return object;
}

} // namespace xui
