#pragma once

#include "widgets/widget.hh"
#include "core/layout_tree.hh"

namespace xui
{

class Text final
{
public:
    Text( float font_size,
          const sf::Font* font,
          std::string init_string)
        : font_size_{ font_size}
        , font_{ font}
        , string_{ std::move( init_string)}
    {
        assert( font && font_size > 0);
    }

    Text( const Text&) = delete;
    Text& operator=( const Text&) = delete;
    Text( Text&&) = delete;
    Text& operator=( Text&&) = delete;
    ~Text() = default;

public:
    void setString( std::string new_string) { string_ = std::move( new_string); }
    const std::string& getString() const { return string_; } 

    const sf::Font* getFont() const { return font_; }
    float getFontSize() const { return font_size_; }

private:
    float font_size_;
    const sf::Font* font_;
    std::string string_;
};

inline void
Render( const Text& text,
        const Geometry& geometry,
        sf::RenderTarget& target)
{
    sf::RectangleShape back;
    back.setPosition( geometry.tl());
    back.setFillColor( sf::Color::White);
    back.setSize( geometry.size());

    sf::Text render_text;
    render_text.setFont( *text.getFont());
    render_text.setString( text.getString());
    render_text.setPosition( geometry.tl());
    render_text.setCharacterSize( text.getFontSize());
    render_text.setFillColor( sf::Color::Blue);

    target.draw( back);
    target.draw( render_text);
}

inline LayoutObject
Layout( const Text& text,
        const Constraints& cons)
{
    Geometry geom{ {0, 0}, cons};
    LayoutObject self{ text, std::move( geom)};

    return self;
}

} // namespace xui
