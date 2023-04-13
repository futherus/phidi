#pragma once

#include "layout_tree.hh"
#include "debug.hh"

namespace xui
{

// 0)
// WidgetPtr
// Button*

// 1)
// Widget
// WidgetPtr<Button>

// 2)
// WidgetRef
// Button&

// template <typename isConst>

class WidgetRef final
{
public:
    template<typename WidgetT>
    WidgetRef( WidgetT& widget)
        : widget_{ std::addressof( widget) }
        , layout_{ []( void* widgetBytes, const Constraints& cons)
            {$FUNC
                auto* tmp = static_cast<WidgetT*>( widgetBytes);
                return Layout( *tmp, cons);
            }
          }
    {}

    friend LayoutObject Layout( const WidgetRef& widget, const Constraints& cons)
    {$FUNC
        return widget.layout_( widget.widget_, cons);
    }

private:
    using LayoutOperation = LayoutObject( void*, const Constraints& cons);

    void* widget_;
    LayoutOperation* layout_;
};

#if 0
class WidgetConstRef final
{
public:
    template<typename WidgetT>
    WidgetConstRef( const WidgetT& widget)
        : widget_{ std::addressof( widget) }
        , layout_{ []( const void* widgetBytes, const Constraints& cons)
            {
                auto* tmp = static_cast<const WidgetT*>( widgetBytes);
                return Layout( *tmp, cons);
            }
          }
    {}

    friend LayoutObject Layout( const WidgetConstRef& widget, const Constraints& cons)
    {
        return widget.layout_( widget.widget_, cons);
    }

private:
    using LayoutOperation = LayoutObject( const void*, const Constraints& cons);

    const void* widget_;
    LayoutOperation* layout_;
};
#endif

#if 0
class Widget final
{
public:
    Widget( Widget&& other) = default;
    Widget& operator=( Widget&& other) = default;

    // Widget( const Widget& other) = delete;

    Widget( const Widget& other)
        : impl_{ other.impl_->clone()}
    {}

    Widget& operator=( const Widget& other)
    {
        other.impl_->clone().swap( impl_);
        return *this;
    }

private:
    struct WidgetConcept
    {
        virtual ~WidgetConcept() = default;
        virtual std::unique_ptr<WidgetConcept> clone() const = 0;
        virtual const void* address() const = 0;
        virtual LayoutObject layout( const Constraints& cons) const = 0;
    };

    template <typename T>
    class WidgetModel final
        : public WidgetConcept
    {
    public:
        WidgetModel( T widget)
            : widget_{ std::move( widget)}
        {}

        std::unique_ptr<WidgetConcept> clone() const override
        {
            return std::make_unique<WidgetModel<T>>( *this);
        }

        LayoutObject layout( const Constraints& cons) const override
        {
            return Layout( widget_, cons);
        }

        const void* address() const override
        {
            return static_cast<const void*>( widget_.address());
        }

    private:
        T widget_;
    };

public:
    using Concept = WidgetConcept;

    template <typename T>
    Widget( T widget)
        : impl_{ std::make_unique<WidgetModel<T>>( std::move( widget))}
    {}

    template <typename T>
    bool operator==( const T& other)
    {
        return impl_->address() == other.address();
    }

    friend LayoutObject
    Layout( Widget widget, const Constraints& cons)
    {
        return widget.impl_->layout( cons);
    }

private:
    std::unique_ptr<WidgetConcept> impl_;
};
#endif

#if 0

template <typename T>
class Impl
{
// FIXME: Cannot be made private)
protected:
    ~Impl() = default;

public:
    Impl() = default;
    Impl( T* impl)
        : impl_{ impl}
    {}

    Impl( const Impl& other) = default;
    Impl& operator=( const Impl& other) = default;

    Impl( Impl&& other) = default;
    Impl& operator=( Impl&& other) = default;

    const T* address() const { return impl_; }
    // void reset( T* new_impl) { impl_ = new_impl; }

    bool operator==( const Impl& rhs)
    {
        return impl_ == rhs.impl_;
    }

protected:
          T& impl()       { return *impl_; }
    const T& impl() const { return *impl_; }

private:
    T* impl_;
};
#endif


} // namespace xui
