#include "pallette.hh"

namespace xui
{

void PushPallette::add( PushButton button)
{
    size_t sz = getColumn().getWidgets().size();
    button.bind( [=]( bool val){ this->onChange( val, sz); });

    this->getColumn().add( button);
}

void PushPallette::update( int new_state)
{$FUNC
    impl().active_button_ = new_state;

    auto col = getColumn();
    for ( size_t i = 0; i < col.getWidgets().size(); i++ )
        col.getWidgets().at( i).update( false);

    col.getWidgets().at( impl().active_button_).update( true);
}

void PushPallette::onChange( bool is_pressed,
                             int button_index)
{
    if ( is_pressed == false )
    {
        $D( "Button is already pressed\n");
        return;
    }

    impl().on_change_( button_index);
}

void
Render( PushPallette pallette,
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
Layout( PushPallette pallette,
        const Constraints& cons)
{$FUNC
    LayoutObject object{ pallette, Geometry{{}, cons}, 1};

    object.push_back( Layout( pallette.getColumn(), cons));

    $M( "returning PushPallette (%f, %f) (%f, %f)\n", object.getPosition().x, object.getPosition().y, object.getSize().x, object.getSize().y);
    return object;
}

} // namespace xui
