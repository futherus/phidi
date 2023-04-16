#pragma once

#include <type_traits>

#include "layout_tree.hh"
#include "debug.hh"

namespace xui
{

class WidgetRef final
{
public:
    WidgetRef( const WidgetRef& other) = delete;
    WidgetRef& operator=( const WidgetRef& other) = delete;

    WidgetRef( WidgetRef&& other) = default;
    WidgetRef& operator=( WidgetRef&& other) = default;

    ~WidgetRef() = default;

    //
    // We have to protect from non-const copy constructor because
    // the instantiated template version with BoolControlT=BoolControlRef is incorrect.
    //
    template<typename WidgetT,
             std::enable_if_t<!std::is_same<WidgetRef, std::decay_t<WidgetT>>::value, bool> = true>
    WidgetRef( WidgetT& widget)
        : widget_{ std::addressof( widget) }
        , layout_{ []( void* widget_bytes, const Constraints& cons)
                   {$FUNC
                       auto* tmp = static_cast<WidgetT*>( widget_bytes);
                       return Layout( *tmp, cons);
                   }}
    {}

    friend LayoutObject
    Layout( const WidgetRef& widget, const Constraints& cons)
    {$FUNC
        return widget.layout_( widget.widget_, cons);
    }

private:
    using LayoutOperation = LayoutObject( void*, const Constraints& cons);

    void* widget_;
    LayoutOperation* layout_;
};

} // namespace xui
