#pragma once

#include "layout_tree.hh"
#include "debug.hh"

namespace xui
{

class WidgetPtr final
{
private:
    struct WidgetConcept
    {
        virtual ~WidgetConcept() = default;
        virtual LayoutObject layout( const Constraints& cons) const = 0;
    };

    template <typename T>
    class WidgetModel final
        : public WidgetConcept
    {
    public:
        WidgetModel( const T* widget)
            : widget_{ widget}
        {
            static_assert( !std::is_same<T, WidgetPtr>::value);
            assert( widget_);
            $FUNC
            // $D( "My destructor: %p\n", std::addressof(~WidgetModel));
        }

        ~WidgetModel() override
        {$FUNC}

        virtual LayoutObject layout( const Constraints& cons) const override
        {$FUNC
            assert( widget_);
            return Layout( widget_, cons);
        }

    private:
        const T* widget_;
    };

public:
    template <typename T>
    WidgetPtr( const T* widget)
        : impl_{ std::make_unique<WidgetModel<T>>( widget)}
    {}

    WidgetPtr( WidgetPtr&& other) = default;
    WidgetPtr& operator=( WidgetPtr&& other) = default;

    ~WidgetPtr()
    {$FUNC}

    friend LayoutObject
    Layout( const WidgetPtr& widget, const Constraints& cons)
    {$FUNC
        assert( widget.impl_.get());
        LayoutObject object = widget.impl_->layout( cons);
        $M( "returning WidgetPtr (%f, %f) (%f, %f)\n", object.getPosition().x, object.getPosition().y, object.getSize().x, object.getSize().y);

        return object;
    }

private:
    std::unique_ptr<WidgetConcept> impl_;
};

} // namespace xui
