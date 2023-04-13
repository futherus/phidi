#pragma once

#include "debug.hh"
#include "button.hh"
#include "column.hh"

namespace xui
{

class PushPallette
{
private:
    Column<PushButton*> column_;
    std::function<void( int)> on_change_;
    int active_button_;

public:
    PushPallette( int padding)
        : column_{ padding}
        , on_change_{}
        , active_button_{}
    {}

    void bind( std::function<void( int)>&& func)
    {
        on_change_ = std::move( func);
    }

    void add( PushButton* button);

    void update( int new_state);
    void onChange( bool new_state, int index);

    const Column<PushButton*>* getColumn() const { return &column_; }
};

void Render( const PushPallette& pallette, const Geometry& geometry, sf::RenderTarget& target);

LayoutObject Layout( const PushPallette* pallette, const Constraints& cons);

} // namespace xui
