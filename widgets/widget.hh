#pragma once

#include <type_traits>

#include "core/layout_tree.hh"
#include "core/debug.hh"

namespace xui
{

class LayoutDelegate final
{
public:
    using This = const void*;
    using LayoutOperation = LayoutObject( This, const Constraints& cons);

public:
    LayoutDelegate( const LayoutDelegate& other) = delete;
    LayoutDelegate& operator=( const LayoutDelegate& other) = delete;

    LayoutDelegate( LayoutDelegate&& other) = default;
    LayoutDelegate& operator=( LayoutDelegate&& other) = default;

    ~LayoutDelegate() = default;

    //
    // We have to protect from non-const copy constructor because
    // the instantiated template version with LayoutableT=LayoutDelegate is incorrect.
    //
    template<typename LayoutableT,
             std::enable_if_t<!std::is_same<LayoutDelegate, std::decay_t<LayoutableT>>::value, bool> = true>
    LayoutDelegate( LayoutableT& widget)
        : widget_{ std::addressof( widget) }
        , layout_{ []( This widget_bytes, const Constraints& cons)
                   {$FUNC
                       auto* tmp = static_cast<const LayoutableT*>( widget_bytes);
                       return Layout( *tmp, cons);
                   }}
    {}

    friend LayoutObject
    Layout( const LayoutDelegate& widget, const Constraints& cons)
    {$FUNC
        return widget.layout_( widget.widget_, cons);
    }

private:
    This widget_;
    LayoutOperation* layout_;
};

} // namespace xui
