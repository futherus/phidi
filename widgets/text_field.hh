#pragma once

#include <functional>
#include "widgets/widget.hh"
#include "core/layout_tree.hh"
#include "core/event.hh"

namespace xui
{

class TextField final
{
public:
    TextField( float font_size,
               const sf::Font* font,
               std::string init_string)
        : font_size_{ font_size}
        , font_{ font}
        , string_{ std::move( init_string)}
        , has_focus_{ false}
        , on_submitted_{}
        , on_changed_{}
    {
        assert( font && font_size > 0);

        auto& ev_mngr = EventManager::instance();
        ev_mngr.subOnMousePressed( *this);
        ev_mngr.subOnKeyPressed( *this);
        ev_mngr.subOnTextEntered( *this);
    }

    TextField( const TextField&) = delete;
    TextField& operator=( const TextField&) = delete;
    TextField( TextField&&) = delete;
    TextField& operator=( TextField&&) = delete;
    ~TextField() = default;

public:
    void bindOnSubmitted( std::function<void( void)>&& func) { on_submitted_ = std::move( func); }
    void bindOnChanged  ( std::function<void( void)>&& func) { on_changed_   = std::move( func); }

    void onMousePressed( sf::Vector2f) { has_focus_ = true; }
    void onKeyPressed( sf::Keyboard::Key code);
    void onTextEntered( uint32_t utf32);

public:
    // FIXME: on_changed_()?
    void setString( std::string new_string) { string_ = std::move( new_string); }
    const std::string& getString() const { return string_; } 

    const sf::Font* getFont() const { return font_; }
    float getFontSize() const { return font_size_; }

    bool hasFocus() const { return has_focus_; }

private:
    std::string string_;
    const sf::Font* font_;
    float font_size_;

    bool has_focus_;

    std::function<void( void)> on_submitted_;
    std::function<void( void)> on_changed_;
};

void Render( const TextField& text, const Geometry& geometry, sf::RenderTarget& target);
LayoutObject Layout( const TextField& text, const Constraints& cons);

} // namespace xui
