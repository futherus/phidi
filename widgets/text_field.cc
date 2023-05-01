#include "text_field.hh"

namespace xui
{

void
TextField::onKeyPressed( sf::Keyboard::Key code)
{
    switch (code)
    {
        case sf::Keyboard::Enter:
            on_submitted_();
            has_focus_ = false;
            break;

        case sf::Keyboard::Backspace:
            if (string_.length())
            {
                string_.pop_back();
                on_changed_();
            }
            break;

        default:
            break;
    }
}

void
TextField::onTextEntered( uint32_t utf32)
{
    // FIXME: utf?
    assert( utf32 < 128U && "only ASCII is supported");

    if ( isprint( utf32))
    {
        string_.push_back( static_cast<char>( utf32));
        on_changed_();
    }
}

inline void
Render( const TextField& text,
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
    render_text.setFillColor( sf::Color::Green);

    target.draw( back);
    target.draw( render_text);
}

inline LayoutObject
Layout( const TextField& text,
        const Constraints& cons)
{
    Geometry geom{ {0, 0}, cons};
    LayoutObject self{ text, std::move( geom)};

    return self;
}


} // namespace xui
