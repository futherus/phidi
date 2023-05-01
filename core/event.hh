#pragma once

#include <functional>
#include <unordered_map>
#include <SFML/Window/Event.hpp>

#include "layout_tree.hh"

namespace xui
{

class MousePressedDelegate final
{
public:
    using This = void*;
    using OnMousePressedOp = void ( This, sf::Vector2f);

public:
    MousePressedDelegate( const MousePressedDelegate& other) = delete;
    MousePressedDelegate& operator=( const MousePressedDelegate& other) = delete;

    MousePressedDelegate( MousePressedDelegate&& other) = default;
    MousePressedDelegate& operator=( MousePressedDelegate&& other) = default;

    ~MousePressedDelegate() = default;

    //
    // We have to protect from non-const copy constructor because
    // the instantiated template version with ClickableT=MousePressedDelegate is incorrect.
    //
    template<typename ClickableT,
             std::enable_if_t<!std::is_same<MousePressedDelegate, std::decay_t<ClickableT>>::value, bool> = true>
    MousePressedDelegate( ClickableT& clickable)
        : clickable_{ std::addressof( clickable)}
        , on_mouse_pressed_{ []( This clickable_bytes, sf::Vector2f rel_pos)
                             {
                                 auto* tmp = static_cast<ClickableT*>( clickable_bytes);
                                 tmp->onMousePressed( rel_pos);
                             }}
    {}

    void
    onMousePressed( sf::Vector2f rel_pos) const
    {
        on_mouse_pressed_( clickable_, rel_pos);
    }

private:
    This clickable_;
    OnMousePressedOp* on_mouse_pressed_;
};

class MouseReleasedDelegate final
{
public:
    using This = void*;
    using OnMouseReleasedOp = void ( This, sf::Vector2f);

public:
    MouseReleasedDelegate( const MouseReleasedDelegate& other) = delete;
    MouseReleasedDelegate& operator=( const MouseReleasedDelegate& other) = delete;

    MouseReleasedDelegate( MouseReleasedDelegate&& other) = default;
    MouseReleasedDelegate& operator=( MouseReleasedDelegate&& other) = default;

    ~MouseReleasedDelegate() = default;

    //
    // We have to protect from non-const copy constructor because
    // the instantiated template version with ClickableT = MouseReleasedDelegate is incorrect.
    //
    template<typename ClickableT,
             std::enable_if_t<!std::is_same<MouseReleasedDelegate, std::decay_t<ClickableT>>::value, bool> = true>
    MouseReleasedDelegate( ClickableT& clickable)
        : clickable_{ std::addressof( clickable)}
        , on_mouse_released_{ []( This clickable_bytes, sf::Vector2f rel_pos)
                              {
                                  auto* tmp = static_cast<ClickableT*>( clickable_bytes);
                                  tmp->onMouseReleased( rel_pos);
                              }}
    {}

    void
    onMouseReleased( sf::Vector2f rel_pos) const
    {
        on_mouse_released_( clickable_, rel_pos);
    }

private:
    This clickable_;
    OnMouseReleasedOp* on_mouse_released_;
};

class MouseMovedDelegate final
{
public:
    using This = void*;
    using OnMouseMovedOp = void ( This, sf::Vector2f);

public:
    MouseMovedDelegate( const MouseMovedDelegate& other) = delete;
    MouseMovedDelegate& operator=( const MouseMovedDelegate& other) = delete;

    MouseMovedDelegate( MouseMovedDelegate&& other) = default;
    MouseMovedDelegate& operator=( MouseMovedDelegate&& other) = default;

    ~MouseMovedDelegate() = default;

    //
    // We have to protect from non-const copy constructor because
    // the instantiated template version with T = MouseMovedDelegate is incorrect.
    //
    template<typename T,
             std::enable_if_t<!std::is_same<MouseMovedDelegate, std::decay_t<T>>::value, bool> = true>
    MouseMovedDelegate( T& object)
        : object_{ std::addressof( object)}
        , on_mouse_moved_{ []( This object_bytes, sf::Vector2f rel_pos)
                           {
                               auto* tmp = static_cast<T*>( object_bytes);
                               tmp->onMouseMoved( rel_pos);
                           }}
    {}

    void
    onMouseMoved( sf::Vector2f rel_pos) const
    {
        on_mouse_moved_( object_, rel_pos);
    }

private:
    This object_;
    OnMouseMovedOp* on_mouse_moved_;
};

class TextEnteredDelegate final
{
public:
    using This = void*;
    using OnTextEnteredOp = void ( This, uint32_t utf32);
    using HasFocusOp = bool ( const This);

public:
    TextEnteredDelegate( const TextEnteredDelegate& other) = delete;
    TextEnteredDelegate& operator=( const TextEnteredDelegate& other) = delete;

    TextEnteredDelegate( TextEnteredDelegate&& other) = default;
    TextEnteredDelegate& operator=( TextEnteredDelegate&& other) = default;

    ~TextEnteredDelegate() = default;

    //
    // We have to protect from non-const copy constructor because
    // the instantiated template version with T = TextEnteredDelegate is incorrect.
    //
    template<typename T,
             std::enable_if_t<!std::is_same<TextEnteredDelegate, std::decay_t<T>>::value, bool> = true>
    TextEnteredDelegate( T& object)
        : object_{ std::addressof( object)}
        , on_text_entered_{ []( This object_bytes, uint32_t utf32)
                            {
                                auto* tmp = static_cast<T*>( object_bytes);
                                tmp->onTextEntered( utf32);
                            }}
        , has_focus_{ []( const This object_bytes)
                      {
                          auto* tmp = static_cast<const T*>( object_bytes);
                          return tmp->hasFocus();
                      }}
    {}

    void
    onTextEntered( uint32_t utf32) const
    {
        on_text_entered_( object_, utf32);
    }

    bool
    hasFocus() const
    {
        return has_focus_( object_);
    }

private:
    This object_;
    OnTextEnteredOp* on_text_entered_;
    HasFocusOp* has_focus_;
};

class KeyPressedDelegate final
{
public:
    using This = void*;
    using OnKeyPressedOp = void ( This, sf::Keyboard::Key code);
    using HasFocusOp = bool ( const This);

public:
    KeyPressedDelegate( const KeyPressedDelegate& other) = delete;
    KeyPressedDelegate& operator=( const KeyPressedDelegate& other) = delete;

    KeyPressedDelegate( KeyPressedDelegate&& other) = default;
    KeyPressedDelegate& operator=( KeyPressedDelegate&& other) = default;

    ~KeyPressedDelegate() = default;

    //
    // We have to protect from non-const copy constructor because
    // the instantiated template version with T = KeyPressedDelegate is incorrect.
    //
    template<typename T,
             std::enable_if_t<!std::is_same<KeyPressedDelegate, std::decay_t<T>>::value, bool> = true>
    KeyPressedDelegate( T& object)
        : object_{ std::addressof( object)}
        , on_key_pressed_{ []( This object_bytes, sf::Keyboard::Key code)
                            {
                                auto* tmp = static_cast<T*>( object_bytes);
                                tmp->onKeyPressed( code);
                            }}
        , has_focus_{ []( const This object_bytes)
                      {
                          auto* tmp = static_cast<const T*>( object_bytes);
                          return tmp->hasFocus();
                      }}
    {}

    void
    onKeyPressed( sf::Keyboard::Key code) const
    {
        on_key_pressed_( object_, code);
    }

    bool
    hasFocus() const
    {
        return has_focus_( object_);
    }

private:
    This object_;
    OnKeyPressedOp* on_key_pressed_;
    HasFocusOp* has_focus_;
};

// FIXME: remove sf::Event dependency.
class EventManager
{
public:
    static EventManager& instance()
    {
        static EventManager manager;

        return manager;
    }

    template <typename T>
    void
    subOnMousePressed( T& clickable)
    {
        mouse_pressed_subs_.insert( {std::addressof( clickable), MousePressedDelegate{ clickable}});
    }

    template <typename T>
    void
    subOnMouseReleased( T& clickable)
    {
        mouse_released_subs_.insert( {std::addressof( clickable), MouseReleasedDelegate{ clickable}});
    }

    template <typename T>
    void
    subOnMouseMoved( T& object)
    {
        mouse_moved_subs_.insert( {std::addressof( object), MouseMovedDelegate{ object}});
    }

    template <typename T>
    void
    subOnTextEntered( T& object)
    {
        text_entered_subs_.push_back( TextEnteredDelegate{ object});
    }

    template <typename T>
    void
    subOnKeyPressed( T& object)
    {
        key_pressed_subs_.push_back( KeyPressedDelegate{ object});
    }

    void
    handleEvents( const LayoutObject& lot, sf::Event event)
    {
        switch ( event.type )
        {
            case sf::Event::MouseButtonPressed:
            {
                sf::Vector2f mouse_pos{ event.mouseButton.x, event.mouseButton.y};

                std::vector<const LayoutObject*> clicked;
                std::back_insert_iterator<std::vector<const LayoutObject*>> clicked_it{ clicked};
                lot.find( mouse_pos, clicked_it);

                for ( const LayoutObject* obj : clicked)
                {
                    auto it = mouse_pressed_subs_.find( obj->getWidget());
                    if (it != mouse_pressed_subs_.end())
                        it->second.onMousePressed( mouse_pos - obj->getPosition());
                }

                break;
            }
            case sf::Event::MouseButtonReleased:
            {
                sf::Vector2f mouse_pos{ event.mouseButton.x, event.mouseButton.y};

                std::vector<const LayoutObject*> clicked;
                std::back_insert_iterator<std::vector<const LayoutObject*>> clicked_it{ clicked};
                lot.find( mouse_pos, clicked_it);

                for ( const LayoutObject* obj : clicked)
                {
                    auto it = mouse_released_subs_.find( obj->getWidget());
                    if (it != mouse_released_subs_.end())
                        it->second.onMouseReleased( mouse_pos - obj->getPosition());
                }

                break;
            }
            // FIXME: LostFocus/GainFocus.
            case sf::Event::MouseMoved:
            {
                sf::Vector2f mouse_pos{ event.mouseMove.x, event.mouseMove.y};

                std::vector<const LayoutObject*> objects;
                std::back_insert_iterator<std::vector<const LayoutObject*>> objects_it{ objects};
                lot.find( mouse_pos, objects_it);

                for ( const LayoutObject* obj : objects)
                {
                    // FIXME: Do we need onMouseMoved for widgets that are not under mouse?
                    auto it = mouse_moved_subs_.find( obj->getWidget());
                    if (it != mouse_moved_subs_.end())
                        it->second.onMouseMoved( mouse_pos - obj->getPosition());
                }

                break;
            }
            case sf::Event::TextEntered:
            {
                for ( const TextEnteredDelegate& delegate : text_entered_subs_)
                {
                    if ( delegate.hasFocus())
                        delegate.onTextEntered( event.text.unicode);
                }

                break;
            }
            case sf::Event::KeyPressed:
            {
                for ( const KeyPressedDelegate& delegate : key_pressed_subs_)
                {
                    if ( delegate.hasFocus())
                        delegate.onKeyPressed( event.key.code);
                }

                break;
            }
            default:
            {
                break;
            }
        }
    }

private:
    EventManager() = default;

    std::unordered_map<const void*, MousePressedDelegate>  mouse_pressed_subs_;
    std::unordered_map<const void*, MouseReleasedDelegate> mouse_released_subs_;
    std::unordered_map<const void*, MouseMovedDelegate>    mouse_moved_subs_;

    std::vector<TextEnteredDelegate> text_entered_subs_;
    std::vector<KeyPressedDelegate>  key_pressed_subs_;
};

} // namespace xui
