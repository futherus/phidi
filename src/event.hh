#pragma once

#include <functional>
#include <unordered_map>
#include <SFML/Window/Event.hpp>

#include "layout_tree.hh"

namespace xui
{

class ClickableRef final
{
public:
    ClickableRef( const ClickableRef& other) = delete;
    ClickableRef& operator=( const ClickableRef& other) = delete;

    ClickableRef( ClickableRef&& other) = default;
    ClickableRef& operator=( ClickableRef&& other) = default;

    ~ClickableRef() = default;

    //
    // We have to protect from non-const copy constructor because
    // the instantiated template version with BoolControlT=BoolControlRef is incorrect.
    //
    template<typename ClickableT,
             std::enable_if_t<!std::is_same<ClickableRef, std::decay_t<ClickableT>>::value, bool> = true>
    ClickableRef( const ClickableT& clickable)
        : clickable_{ std::addressof( clickable)}
        , on_mouse_pressed_{ []( const void* clickable_bytes, sf::Vector2f mouse_pos)
                             {
                                 auto* tmp = static_cast<const ClickableT*>( clickable_bytes);
                                 tmp->onMousePressed( mouse_pos);
                             }}
    {}

    void
    onMousePressed( sf::Vector2f mouse_pos) const
    {
        on_mouse_pressed_( clickable_, mouse_pos);
    }

private:
    using OnMousePressedOp = void ( const void*, sf::Vector2f);

    const void* clickable_;
    OnMousePressedOp* on_mouse_pressed_;
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
    subOnMousePressed( const T& clickable)
    {
        clickables_.insert( {std::addressof( clickable), ClickableRef{ clickable}});
    }

    void
    handleEvents( const LayoutObject& lot, sf::Event event)
    {
        switch ( event.type )
        {
            case sf::Event::MouseButtonPressed:
            {
                sf::Vector2f mouse_pos{ event.mouseButton.x, event.mouseButton.y};

                std::vector<const void*> clicked;
                std::back_insert_iterator<std::vector<const void*>> clicked_it{ clicked};
                lot.find( mouse_pos, clicked_it);

                for ( auto* obj : clicked)
                {
                    auto it = clickables_.find( obj);
                    if (it != clickables_.end())
                        it->second.onMousePressed( mouse_pos);
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

    std::unordered_map<const void*, ClickableRef> clickables_;
};

} // namespace xui
