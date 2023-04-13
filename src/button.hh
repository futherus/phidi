#pragma once

#include <assert.h>
#include <array>
#include <functional>
#include "widget.hh"

namespace xui
{

struct PushButtonImpl final
{
    using TexturePack = std::array<std::array<const sf::Texture*, 2>, 2>;

    PushButtonImpl( TexturePack textures, sf::Vector2f size)
        : textures_{ std::move( textures)}
        , on_click_{}
        , size_{ size}
        , is_pushed_{}
        , is_hovered_{}
    {
        assert( textures_[0][0] && textures_[0][1] && textures_[1][0] && textures_[1][1]);
        $M( "Button size: (%f, %f)\n", size.x, size.y);
        // assert( size_ != Vector2f{0, 0});
    }

    TexturePack textures_;
    std::function<void( bool)> on_click_;

    sf::Vector2f size_;

    bool is_pushed_;
    bool is_hovered_;
    // bool is_focused_;
};

class PushButton final
    : public Impl<PushButtonImpl>
{
public:
    using Impl<PushButtonImpl>::Impl;
    // FIXME: Pass functions and using's through Impl?
    using TexturePack = PushButtonImpl::TexturePack;

    void bind( std::function<void( bool)>&& on_click)
    {
        impl().on_click_ = std::move( on_click);
    }

    void update( bool val) { impl().is_pushed_ = val; }

    bool isPushed() const { return impl().is_pushed_; }
    bool isHovered() const { return impl().is_hovered_; }
    sf::Vector2f getSize() const { return impl().size_; }
    const TexturePack& getTextures() const
    {
        assert( impl().textures_[0][0] && impl().textures_[0][1] && impl().textures_[1][0] && impl().textures_[1][1]);
        $M( "Textures: %p, %p, %p, %p\n", impl().textures_[0][0], impl().textures_[0][1], impl().textures_[1][0], impl().textures_[1][1]);
        return impl().textures_;
    }
};

void onMousePressed( PushButton, const sf::Event&);
void onMouseMoved(   PushButton, const sf::Event&);

void Render( PushButton button, const Geometry& geometry, sf::RenderTarget& target);
LayoutObject Layout( PushButton button, const Constraints& cons);

} // namespace xui
