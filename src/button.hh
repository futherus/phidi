#pragma once

#include <assert.h>
#include <array>
#include <functional>
#include "event.hh"
#include "widget.hh"

namespace xui
{

class PushButton final
{
public:
    using TexturePack = std::array<std::array<const sf::Texture*, 2>, 2>;

    PushButton( TexturePack textures, sf::Vector2f size)
        : textures_{ std::move( textures)}
        , on_click_{}
        , size_{ size}
        , is_pushed_{}
        , is_hovered_{}
    {
        assert( textures_[0][0] && textures_[0][1] && textures_[1][0] && textures_[1][1]);

        EventManager::instance().subOnMousePressed( *this);
        $M( "Button size: (%f, %f)\n", size.x, size.y);
    }

    PushButton( const PushButton&) = delete;
    PushButton& operator=( const PushButton&) = delete;
    PushButton( PushButton&&) = delete;
    PushButton& operator=( PushButton&&) = delete;
    ~PushButton() = default;

    void bind( std::function<void( bool)>&& on_click) { on_click_ = std::move( on_click); }
    void onMousePressed( sf::Vector2f /* mouse_pos */) const { on_click_( !is_pushed_); }

    void update( bool val) { is_pushed_ = val; }

    bool isPushed() const { return is_pushed_; }
    bool isHovered() const { return is_hovered_; }
    sf::Vector2f getSize() const { return size_; }
    const TexturePack& getTextures() const { return textures_; }

private:
    TexturePack textures_;
    std::function<void( bool)> on_click_;

    sf::Vector2f size_;

    bool is_pushed_;
    bool is_hovered_;
    // bool is_focused_;
};

// void onMouseMoved(   const PushButton&, const sf::Event&);

void Render( const PushButton& button, const Geometry& geometry, sf::RenderTarget& target);
LayoutObject Layout( const PushButton& button, const Constraints& cons);

} // namespace xui
