#pragma once

#include <cassert>
#include <array>
#include <functional>

#include "core/event.hh"
#include "widgets/widget.hh"

namespace xui
{

class PushButton final
{
public:
    using TexturePack = std::array<std::array<const sf::Texture*, 2>, 2>;

public:
    PushButton( TexturePack textures)
        : textures_{ std::move( textures)}
        , on_click_{}
        , is_pushed_{}
        , is_hovered_{}
    {
        assert( textures_[0][0] && textures_[0][1] && textures_[1][0] && textures_[1][1]);

        EventManager::instance().subOnMousePressed( *this);
    }

    PushButton( const PushButton&) = delete;
    PushButton& operator=( const PushButton&) = delete;
    PushButton( PushButton&&) = delete;
    PushButton& operator=( PushButton&&) = delete;
    ~PushButton() = default;

public:
    void bind( std::function<void( bool)>&& on_click) { on_click_ = std::move( on_click); }
    void onMousePressed( sf::Vector2f /* mouse_pos */) const { verify(); on_click_( !is_pushed_); }
    void onMouseReleased( sf::Vector2f /* mouse_pos */) const { verify(); }
    void update( bool val) { verify(); is_pushed_ = val; }

    bool isPushed() const { verify(); return is_pushed_; }
    bool isHovered() const { verify(); return is_hovered_; }
    const TexturePack& getTextures() const { return textures_; }

    void
    verify() const
    {
        assert( on_click_ && "Function wasn't binded");
    }

private:
    TexturePack textures_;
    std::function<void( bool)> on_click_;

    bool is_pushed_;
    bool is_hovered_;
    // bool is_focused_;
};

// void onMouseMoved(   const PushButton&, const sf::Event&);

Json DumpInfo( const PushButton& button);
void Render( const PushButton& button, const Geometry& geometry, sf::RenderTarget& target);
LayoutObject Layout( const PushButton& button, const Constraints& cons);

} // namespace xui
