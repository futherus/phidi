#ifndef BIND_HH
#define BIND_HH

#include <cstdio>
#include <memory>

namespace xui
{

template <typename TCtlArg>
class IBind
{
public:
    virtual ~IBind() = default;

    virtual void operator()( TCtlArg) = 0;
};

template <typename TObject, typename TCtlArg, typename TExtraArg>
class Bind
    : public IBind<TCtlArg>
{
public:
    using Method = void (TObject::*)( TCtlArg, TExtraArg);

private:
    TObject* object_;
    Method method_;
    TExtraArg extra_arg_;

public:
    Bind( TObject& object, Method method, TExtraArg extra_arg)
        : IBind<TCtlArg>{}
        , object_{ &object}
        , method_{ method}
        , extra_arg_{ extra_arg}
    {}

    virtual void operator()( TCtlArg arg) override
    {
        (object_->*method_)( arg, extra_arg_);
    }
};

// class IView
// {
// public:
//     virtual ~IView() = default;

//     virtual void update() = 0;
// };

// class Button
// {
// private:
//     std::unique_ptr<IBind<int>> on_click_;

// public:
//     void onClick()
//     {
//         int temp = 0;
//         (*on_click_)( temp);
//     }

//     template <class TObject, typename TExtraArg>
//     void bind( TObject& object,
//                typename Bind<TObject, int, TExtraArg>::Method method,
//                TExtraArg targ)
//     {
//         on_click_ = std::make_unique<Bind<TObject, int, TExtraArg>>( object, method, targ);
//     }
// };

// class view_tmp
// {
// public:
//     Button btn1;

//     view_tmp()
//     {
//         btn1.bind(*this, &view_tmp::setTool_Bind, 123);
//     }

//     void setTool_Bind(int who, int val)
//     {
//         std::printf("who = %d; val = %d\n", who, val);
//     };
// };

} // namespace xui

#endif // BIND_HH
