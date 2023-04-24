#include "widgets/palette.hh"

namespace xui
{

int
PushPalette::add( LayoutDelegate&& layout,
                  float flex,
                  BoolControlDelegate&& button)
{
    int index = getControls().size();
    button.bind( [=]( bool val){ this->onChange( val, index); });

    getControls().push_back( std::move( button));

    getColumn().push_back( std::move( layout), flex);

    return index;
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
    verify();

    if ( is_pressed == false )
    {
        $D( "Button is already pressed\n");
        return;
    }

    on_change_( button_index);
}

void
Render( const PushPalette& palette,
        const Geometry& geometry,
        sf::RenderTarget& target)
{
    palette.verify();

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
    palette.verify();

    LayoutObject object{ palette, 1};

    $$
    LayoutObject col = Layout( palette.getLayout(), cons);
    object.setSize( col.getSize());
    object.push_back( std::move( col));

    $M( "returning PushPalette (%f, %f) (%f, %f)\n", object.getPosition().x, object.getPosition().y, object.getSize().x, object.getSize().y);
    return object;
}

} // namespace xui
