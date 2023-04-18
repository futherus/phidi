#include "pallette.hh"

namespace xui
{

void
PushPallette::add( BoolControlDelegate&& button)
{
    size_t sz = column_.size();
    button.bind( [=]( bool val){ this->onChange( val, sz); });

    column_.push_back( std::move( button));
}

void
PushPallette::update( int new_state)
{
    active_button_ = new_state;

    for ( auto& button : column_ )
        button.update( false);

    column_.at( active_button_).update( true);
}

void
PushPallette::onChange( bool is_pressed,
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
Render( const PushPallette&,
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
Layout( const PushPallette& pallette,
        const Constraints& cons)
{$FUNC
    LayoutObject object{ pallette, 1};

    $$
    object.push_back( Layout( pallette.getColumn(), cons));

    object.setSize( object.back().getSize());

    $M( "returning PushPallette (%f, %f) (%f, %f)\n", object.getPosition().x, object.getPosition().y, object.getSize().x, object.getSize().y);
    return object;
}

} // namespace xui
