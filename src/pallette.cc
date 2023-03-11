#include "pallette.hh"

namespace xui
{

void PushPallette::adjustSprite()
{
    // sprite_.setTexture( *texture_);
    // sprite_.setBounds( bounds());
}

PushPallette::PushPallette( const Rectangle& bounds)
    : WidgetManager{ bounds}
{
    // FIXME: bounds vs width
}

void PushPallette::bind( std::function<void(int)>&& func)
{
    on_change_ = std::move(func);
}

void PushPallette::add( PushButton* button)
{
    size_t sz = size();
    button->bind( [=]( bool val){ this->onChange(val, sz); });

    this->WidgetManager::add( button);
}

void PushPallette::update( int new_state)
{
    fprintf( stderr, "%s called\n", __PRETTY_FUNCTION__);
    active_button_ = new_state;

    for (size_t i = 0; i < size(); i++)
        widgets_.at( i)->update( false);

    widgets_.at( active_button_)->update( true);
}

void PushPallette::onChange( bool is_pressed,
                             int button_index)
{
    if (is_pressed == false)
    {
        fprintf( stderr, "Button is already pressed\n");
        return;
    }

    on_change_( button_index);
}

void PushPallette::draw( sf::RenderTarget& target) const
{
    // sprite_.draw( target);

    WidgetManager::draw( target);
}

} // namespace xui
