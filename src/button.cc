#include "button.hh"

namespace xui
{

void PushButton::adjustTexture()
{
    const sf::Texture* texture = textures_[is_hovered_][is_pushed_];
    assert(texture);
    sprite_.setTexture( *texture, true);
    sprite_.setPosition( bounds().tl());

    // texture_->adjust({bounds().left(), bounds().top()}, is_hovered_, is_pushed_);
}

PushButton::PushButton( const Rectangle& bounds,
                        TexturePack textures)
    : IWidget{ bounds}
    , on_click_{}
    , textures_{ textures}
    , sprite_{}
    , is_pushed_{}
    , is_hovered_{}
{
    adjustTexture();
}

void PushButton::bind(std::function<void(bool)>&& func)
{
    on_click_ = std::move(func);
}

void PushButton::update(bool new_state)
{
    is_pushed_ = new_state;

    adjustTexture();
}

void PushButton::onMousePressed(const sf::Event&)
{
    fprintf( stderr, "%s\n", __PRETTY_FUNCTION__);

    // (*on_click_)( !is_pushed_);

    on_click_( !is_pushed_);

    // assert(slot_ && "slot was not set");
    // slot_->slot(this, !is_pushed_);
}

void PushButton::onMouseMoved(const sf::Event& event)
{
    float x = static_cast<float>(event.mouseMove.x);
    float y = static_cast<float>(event.mouseMove.y);

    is_hovered_ = contains({x, y});

    adjustTexture();
}

void PushButton::draw(sf::RenderTarget& target) const
{
    target.draw(sprite_);

    this->IWidget::draw(target);
}

} // namespace xui