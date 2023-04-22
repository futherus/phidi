#include "widgets/palette.hh"

namespace xui
{

void
PushPalette::add( LayoutDelegate&& layout,
                  BoolControlDelegate&& button)
{
    size_t sz = getControls().size();
    button.bind( [=]( bool val){ this->onChange( val, sz); });

    getControls().push_back( std::move( button));

    getColumn().getChild().push_back( std::move( layout));
}

void
PushPalette::update( int new_state)
{
    active_button_ = new_state;

    for ( auto& button : getControls() )
        button.update( false);

    getControls().at( active_button_).update( true);
}

void
PushPalette::onChange( bool is_pressed,
                       int button_index)
{
    if ( is_pressed == false )
    {
        $D( "Button is already pressed\n");
        return;
    }

    on_change_( button_index);
}

void
Render( const PushPalette&,
        const Geometry& geometry,
        sf::RenderTarget& target)
{
    sf::RectangleShape background;
    background.setSize( geometry.size());
    background.setFillColor( sf::Color::Green);
    background.setPosition( geometry.tl());
    target.draw( background);
}

LayoutObject
Layout( const PushPalette& palette,
        const Constraints& cons)
{$FUNC
    LayoutObject object{ palette, 1};

    $$
    object.push_back( Layout( palette.getColumn(), cons));

    object.setSize( object.back().getSize());

    $M( "returning PushPalette (%f, %f) (%f, %f)\n", object.getPosition().x, object.getPosition().y, object.getSize().x, object.getSize().y);
    return object;
}

} // namespace xui
