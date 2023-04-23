#pragma once

#include <type_traits>
#include "core/debug.hh"

#include "widgets/button.hh"
#include "widgets/column.hh"
#include "widgets/padding.hh"

namespace xui
{

class BoolControlDelegate final
{
public:
    using This = void*;
    using UpdateOp = void( This, bool);
    using BindOp = void( This, std::function<void( bool)>&&);
    using IsPushedOp = bool( This);

public:
    BoolControlDelegate( const BoolControlDelegate& other) = delete;
    BoolControlDelegate& operator=( const BoolControlDelegate& other) = delete;

    BoolControlDelegate( BoolControlDelegate&& other) = default;
    BoolControlDelegate& operator=( BoolControlDelegate&& other) = default;

    ~BoolControlDelegate() = default;

    //
    // We have to protect from non-const copy constructor because the
    // instantiated template version with BoolControlT=BoolControlDelegate is incorrect.
    //
    template <typename BoolControlT,
              std::enable_if_t<!std::is_same<BoolControlDelegate, std::decay_t<BoolControlT>>::value, bool> = true>
    BoolControlDelegate( BoolControlT& control)
        : control_{ std::addressof( control)}
        , update_{ []( This ctl, bool val)
                   {
                       auto* tmp = static_cast<BoolControlT*>( ctl);
                       tmp->update( val);
                   }}
        , bind_{ []( This ctl, std::function<void( bool)>&& on_change)
                 {
                     auto* tmp = static_cast<BoolControlT*>( ctl);
                     tmp->bind( std::move( on_change));
                 }}
        , is_pushed_{ []( This ctl)
                      {
                          auto* tmp = static_cast<const BoolControlT*>( ctl);
                          return tmp->isPushed();
                      }}
    {}

    void update( bool val) { update_( control_, val); }

    void bind( std::function<void( bool)>&& on_click) { bind_( control_, std::move( on_click)); }

    bool isPushed() const { return is_pushed_( control_); }

private:
    This control_;

    UpdateOp* update_;
    BindOp* bind_;
    IsPushedOp* is_pushed_;
};

class PushPalette final
{
public:
    PushPalette( int padding)
        : layout_{ padding, LayoutPolicy{ MainAxisAlignment::Center, CrossAxisAlignment::Center}}
        , on_change_{}
        , active_button_{}
    {
        layout_.setPadding( 15);
    }

    PushPalette( const PushPalette&) = delete;
    PushPalette& operator=( const PushPalette&) = delete;
    PushPalette( PushPalette&&) = delete;
    PushPalette& operator=( PushPalette&&) = delete;
    ~PushPalette() = default;

public:
    void bind( std::function<void( int)>&& func) { on_change_ = std::move( func); }

    void add( LayoutDelegate&& layout, float flex, BoolControlDelegate&& control);

    void update( int new_state);
    void onChange( bool new_state, int index);

    LayoutDelegate getLayout() const { return LayoutDelegate{ layout_}; }

    size_t getSize() const { assert( getControls().size() == getColumn().size()); return getControls().size(); }

private:
    const Column<LayoutDelegate>& getColumn() const { return layout_.getChild(); }
          Column<LayoutDelegate>& getColumn()       { return layout_.getChild(); }

    const std::vector<BoolControlDelegate>& getControls() const { return controls_; }
          std::vector<BoolControlDelegate>& getControls()       { return controls_; }

private:
    std::vector<BoolControlDelegate> controls_;
    Padding<Column<LayoutDelegate>> layout_;

    std::function<void( int)> on_change_;
    int active_button_;
};

void Render( const PushPalette& palette, const Geometry& geometry, sf::RenderTarget& target);

LayoutObject Layout( const PushPalette& palette, const Constraints& cons);

} // namespace xui
