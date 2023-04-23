#pragma once

#include <cmath>
#include "core/layout_tree.hh"
#include "widgets/widget.hh"

namespace xui
{

class Placeholder
{
public:
    Placeholder() = default;

    Placeholder( const Placeholder&) = delete;
    Placeholder& operator=( const Placeholder&) = delete;
    Placeholder( Placeholder&&) = delete;
    Placeholder& operator=( Placeholder&&) = delete;
    ~Placeholder() = default;
};

inline void
Render( const Placeholder&,
        const Geometry& geometry,
        sf::RenderTarget& target)
{
    const float thickness = 5;
    float w = geometry.size().x;
    float h = geometry.size().y;
    assert( w > thickness && h > thickness && "Placeholder needs more space");

    sf::RectangleShape rectangle;
    rectangle.setSize( sf::Vector2f{ w, h});
    rectangle.setFillColor( sf::Color::Transparent);
    rectangle.setOutlineColor( sf::Color::Blue);
    rectangle.setOutlineThickness( -thickness);
    rectangle.setPosition( geometry.left(), geometry.top());
    target.draw( rectangle);

    float len = std::sqrt( w * w + h * h);
    sf::RectangleShape line;
    line.setSize( {len - 2 * thickness, thickness});
    line.setFillColor( sf::Color::Blue);
    line.setOrigin( { -thickness, thickness / 2});

    float angle = std::atan( h / w) / (2 * M_PI) * 360;
    line.rotate( angle);
    line.setPosition( geometry.left(), geometry.top());
    target.draw( line);

    line.rotate( 180 - 2 * angle);
    line.setPosition( geometry.right(), geometry.top());
    target.draw( line);
}

inline LayoutObject
Layout( const Placeholder& padding,
        const Constraints& cons)
{
    Geometry geom{ {0, 0}, cons};
    return LayoutObject{ padding, geom};
}

} // namespace xui
