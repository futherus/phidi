#pragma once

#include "debug.hh"
#include "button.hh"
#include "column.hh"

namespace xui
{

struct PushPalletteImpl final
{
    PushPalletteImpl( int padding)
        : column_{ padding}
        , on_change_{}
        , active_button_{}
    {}

    ColumnImpl<PushButton> column_;
    std::function<void( int)> on_change_;
    int active_button_;
};

class PushPallette final
    : public Impl<PushPalletteImpl>
{
public:
    using Impl::Impl;

    void bind( std::function<void( int)>&& func)
    {
        impl().on_change_ = std::move( func);
    }

    void add( PushButton button);

    void update( int new_state);
    void onChange( bool new_state, int index);

    // const Column<PushButton> getColumn() const { return &impl().column_; }
    Column<PushButton> getColumn() { return &impl().column_; }
};

void Render( PushPallette pallette, const Geometry& geometry, sf::RenderTarget& target);

LayoutObject Layout( PushPallette pallette, const Constraints& cons);

} // namespace xui
