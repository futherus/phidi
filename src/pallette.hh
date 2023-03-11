#ifndef PALLETTE_HH
#define PALLETTE_HH

#include "button.hh"
#include "widget_manager.hh"
// #include "bind.hh"
#include "texture.h"

namespace xui
{

class PushPallette
    : public WidgetManager<PushButton>
{
    // std::unique_ptr<xui::IBind<int>> on_change_;
    std::function<void(int)> on_change_;
    int active_button_;

    // gui::BorderTexture* texture_;
    // gui::BorderSprite sprite_;

    void adjustSprite();

public:
    PushPallette( const Rectangle& bounds);

    // template <class TObject, typename TExtraArg>
    // void bind( TObject& object,
    //            typename xui::Bind<TObject, int, TExtraArg>::Method method,
    //            TExtraArg targ)
    // {
    //     on_change_ = std::make_unique<xui::Bind<TObject, int, TExtraArg>>( object, method, targ);
    // }

    void bind( std::function<void(int)>&& func);
    void add( PushButton* button);

    void update( int new_state);
    void onChange( bool new_state, int index);

    void draw( sf::RenderTarget& target) const override;
};

} // namespace xui

#endif // PALLETTE_HH
