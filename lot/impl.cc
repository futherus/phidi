#include <cstdio>
#include <vector>
#include <memory>

namespace lot { struct LayoutObject {int x;}; }
struct Constraints;

//template <typename T, typename ...Traits>
//class Iface;

using Widget = Iface<WidgetConcept, HasLayout<WidgetConcept>, HasToggle<WidgetConcept>>;

struct Draw
{
    virtual int doit() = 0;
};

class Widget final
{
public:
    Widget( Widget&& other) = default;
    Widget& operator=( Widget&& other) = default;

    Widget( const Widget& other) = delete;
    /*
    Widget( const Widget& other)
        : impl_{ other.impl_->clone()}
    {}

    Widget& operator=( const Widget& other)
    {
        other.impl_->clone().swap( impl_);
        return *this;
    }
    */

private:
    struct WidgetConcept
    {
        virtual ~WidgetConcept() = default;
        virtual std::unique_ptr<WidgetConcept> clone() const = 0;
        virtual const void* address() const = 0;
        virtual lot::LayoutObject layout( const Constraints& cons) const = 0;
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

        lot::LayoutObject layout( const Constraints& cons) const override
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

    friend lot::LayoutObject
    Layout( const Widget& widget, const Constraints& cons)
    {
        return widget.impl_->layout( cons);
    }

private:
    std::unique_ptr<WidgetConcept> impl_;
};








template <typename T>
class Impl
{
public:
    Impl( T* impl): impl_{ impl} {}

    Impl( const Impl& other) = default;
    Impl& operator=( const Impl& other) = default;

    Impl( Impl&& other) = default;
    Impl& operator=( Impl&& other) = default;

    const T* address() const { return impl_; }

    bool operator==( const Impl& rhs)
    {
        return impl_ == rhs.impl_;
    }

private:
    T* impl_;

protected:
          T& impl()       { return *impl_; }
    const T& impl() const { return *impl_; }
};

struct Size { int size; };

struct ButtonImpl
{
    Size size;
    float weight;
    int color;
};

class Button final
    : public Impl<ButtonImpl>
{
public:
    using Impl::Impl;

    Button& setSize( const Size& size)
    {
        impl().size = size;
        return *this;
    }

    Size getSize() const
    {
        return impl().size;
    }
};

struct ColumnImpl
{
    std::vector<Button> buttons;
};

class Column final
    : public Impl<ColumnImpl>
{
public:
    using Impl::Impl;

    void add( const Button& button)
    {
        impl().buttons.push_back( button);
    }
};



int main()
{
    ButtonImpl bi{ };

    Button b1{ &bi};
    Button b2{ &bi};

    Button b = b1;
    if ( b1 == b2 )
    {
    }

    Widget bubik = b2;
    if ( bubik == b2 )
    {
    }


    Button b32kk = b2;

    return 0;
}

