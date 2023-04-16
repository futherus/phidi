#pragma once

#include <type_traits>
#include "debug.hh"
#include "button.hh"
#include "column.hh"

namespace xui
{

class BoolControlRef final
{
public:
    BoolControlRef( const BoolControlRef& other) = delete;
    BoolControlRef& operator=( const BoolControlRef& other) = delete;

    BoolControlRef( BoolControlRef&& other) = default;
    BoolControlRef& operator=( BoolControlRef&& other) = default;

    ~BoolControlRef() = default;

    //
    // We have to protect from non-const copy constructor because
    // the instantiated template version with BoolControlT=BoolControlRef is incorrect.
    //
    template <typename BoolControlT,
              std::enable_if_t<!std::is_same<BoolControlRef, std::decay_t<BoolControlT>>::value, bool> = true>
    BoolControlRef( BoolControlT& control)
        : control_{ std::addressof( control)}
        , layout_{ []( void* ctl, const Constraints& cons)
                   {$FUNC
                       auto* tmp = static_cast<const BoolControlT*>( ctl);
                       return Layout( *tmp, cons);
                   }}
        , update_{ []( void* ctl, bool val)
                   {
                       auto* tmp = static_cast<BoolControlT*>( ctl);
                       tmp->update( val);
                   }}
        , bind_{ []( void* ctl, std::function<void( bool)>&& on_change)
                 {
                     auto* tmp = static_cast<BoolControlT*>( ctl);
                     tmp->bind( std::move( on_change));
                 }}
        , is_pushed_{ []( void* ctl)
                      {
                          auto* tmp = static_cast<const BoolControlT*>( ctl);
                          return tmp->isPushed();
                      }}
    {}

    void update( bool val)
    {
        update_( control_, val);
    }

    void bind( std::function<void( bool)>&& on_click)
    {
        bind_( control_, std::move( on_click));
    }

    bool isPushed() const
    {
        return is_pushed_( control_);
    }

    friend LayoutObject Layout( const BoolControlRef& control, const Constraints& cons)
    {$FUNC
        return control.layout_( control.control_, cons);
    }

    // using This = void*;
    // Great idea!
    using LayoutOp = LayoutObject( void*, const Constraints& cons);
    using UpdateOp = void( void*, bool);
    using BindOp = void( void*, std::function<void( bool)>&&);
    using IsPushedOp = bool( void*);

private:
    void* control_;

    LayoutOp* layout_;
    UpdateOp* update_;
    BindOp* bind_;
    IsPushedOp* is_pushed_;
};

class PushPallette final
{
public:
    PushPallette( int padding)
        : column_{ padding}
        , on_change_{}
        , active_button_{}
    {}

    PushPallette( const PushPallette&) = delete;
    PushPallette& operator=( const PushPallette&) = delete;
    PushPallette( PushPallette&&) = delete;
    PushPallette& operator=( PushPallette&&) = delete;
    ~PushPallette() = default;

    void bind( std::function<void( int)>&& func) { on_change_ = std::move( func); }

    void add( BoolControlRef&& button);

    void update( int new_state);
    void onChange( bool new_state, int index);

    const Column<BoolControlRef>& getColumn() const { return column_; }
          Column<BoolControlRef>& getColumn()       { return column_; }

private:
    Column<BoolControlRef> column_;
    std::function<void( int)> on_change_;
    int active_button_;
};

void Render( const PushPallette& pallette, const Geometry& geometry, sf::RenderTarget& target);

LayoutObject Layout( const PushPallette& pallette, const Constraints& cons);

} // namespace xui
