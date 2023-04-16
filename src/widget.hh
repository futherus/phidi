#pragma once

#include "layout_tree.hh"
#include "debug.hh"

namespace xui
{

class WidgetRef final
{
public:
    //
    // We have to create copy constructor manually because the instantiated
    // template version with WidgetT=WidgetRef is incorrect.
    //
    WidgetRef( WidgetRef& other) = delete;
    WidgetRef( const WidgetRef& other) = default;

    template<typename WidgetT>
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
