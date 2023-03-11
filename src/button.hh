#ifndef BUTTON_HH
#define BUTTON_HH

#include <assert.h>
#include <array>
#include <functional>
// #include "bind.hh"
#include "texture.h"
#include "widget.hh"

namespace xui
{

class PushButton
    : public IWidget
{
public:
    using TexturePack = std::array<std::array<const sf::Texture*, 2>, 2>;

private:
    std::function<void(bool)> on_click_;    

    TexturePack textures_;
    sf::Sprite sprite_;

    bool is_pushed_;
    bool is_hovered_;
    // bool is_focused_;

    void adjustTexture();

public:
    PushButton( const Rectangle& bounds, TexturePack textures);

    // template <class TObject, typename TExtraArg>
    // void bind( TObject& object,
    //            typename xui::Bind<TObject, bool, TExtraArg>::Method method,
    //            TExtraArg extra_arg)
    // {
    //     on_click_ = std::make_unique<xui::Bind<TObject, bool, TExtraArg>>( object, method, extra_arg);
    // }

    void bind(std::function<void(bool)>&& func);

    void update( bool new_state);
    void onMousePressed( const sf::Event&) override;
    void onMouseMoved( const sf::Event& event) override;

    bool isPushed()  const { return is_pushed_; }
    bool isHovered() const { return is_hovered_; }

    void draw( sf::RenderTarget& target) const override;
};

} // namespace xui

#endif // BUTTON_HH