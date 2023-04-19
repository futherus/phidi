#pragma once

#include <type_traits>
#include "core/debug.hh"

#include "widgets/button.hh"
#include "widgets/column.hh"

namespace xui
{

class BoolControlDelegate final
{
public:
    using This = void*;
    using LayoutOp = LayoutObject( This, const Constraints& cons);
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
        , layout_{ []( This ctl, const Constraints& cons)
                   {$FUNC
                       auto* tmp = static_cast<const BoolControlT*>( ctl);
                       return Layout( *tmp, cons);
                   }}
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

    friend LayoutObject
    Layout( const BoolControlDelegate& control, const Constraints& cons)
    {$FUNC
        return control.layout_( control.control_, cons);
    }

private:
    This control_;

    LayoutOp* layout_;
    UpdateOp* update_;
    BindOp* bind_;
    IsPushedOp* is_pushed_;
};

class PushPallette final
{
public:
    PushPallette( int padding)
        : column_{ padding, {MainAxisAlignment::Center, CrossAxisAlignment::Center}}
        , on_change_{}
        , active_button_{}
    {}

    PushPallette( const PushPallette&) = delete;
    PushPallette& operator=( const PushPallette&) = delete;
    PushPallette( PushPallette&&) = delete;
    PushPallette& operator=( PushPallette&&) = delete;
    ~PushPallette() = default;

public:
    void bind( std::function<void( int)>&& func) { on_change_ = std::move( func); }

    void add( BoolControlDelegate&& button);

    void update( int new_state);
    void onChange( bool new_state, int index);

    const Column<BoolControlDelegate>& getColumn() const { return column_; }
          Column<BoolControlDelegate>& getColumn()       { return column_; }

private:
    Column<BoolControlDelegate> column_;
    std::function<void( int)> on_change_;
    int active_button_;
};

void Render( const PushPallette& pallette, const Geometry& geometry, sf::RenderTarget& target);

LayoutObject Layout( const PushPallette& pallette, const Constraints& cons);

} // namespace xui
