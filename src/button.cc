#include "button.hh"
#include "debug.hh"

namespace xui
{

// void
// onMousePressed( PushButton& button,
//                      const sf::Event&)
// {$FUNC
//     button.on_click_( !button.is_pushed_);
// }

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
Render( PushButton button,
        const Geometry& geometry, 
        sf::RenderTarget& target)
{
    $M( "Render: Pointer to button: %p\n", &button);

    // $M( "hov: %d, push: %d\n", (int) button.isHovered(), (int) button.isPushed());
    const sf::Texture* texture = button.getTextures().at( button.isHovered()).at( button.isPushed());
    $M( "Texture: %p\n", texture);
    assert( texture);

    sf::Sprite sprite;
    sprite.setTexture( *texture, true);
    sprite.setPosition( geometry.tl());

    target.draw( sprite);
}

LayoutObject
Layout( PushButton button,
        const Constraints& cons)
{$FUNC
    assert( cons >= button.getSize());

    $M( "Layout: Pointer to button: %p\n", button.address());
    LayoutObject object = LayoutObject{ button, {{}, button.getSize()}};
    $M( "returning PushButton (%f, %f) (%f, %f)\n", object.getPosition().x, object.getPosition().y, object.getSize().x, object.getSize().y);
    return object;
}

} // namespace xui