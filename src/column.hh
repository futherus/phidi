#pragma once

#include "widget.hh"

namespace xui
{

template<typename T>
class Column
{
public:
    Column( int padding)
        : padding_{ padding}
        , widgets_{}
    {}

    Column( const Column&) = delete;
    Column& operator=( const Column&) = delete;

    int getPadding() const { return padding_; }
    const std::vector<T>& getWidgets() const { return widgets_; }
    std::vector<T>& getWidgets() { return widgets_; }
    void add( T widget) { widgets_.push_back( widget); }

private:
    static_assert( std::is_pointer<T>::value || std::is_same<WidgetPtr, T>::value);
    int padding_;
    std::vector<T> widgets_;
};

template<typename T>
inline void
Render( const Column<T>&,
        const Geometry&,
        sf::RenderTarget&)
{}

template<typename T>
inline LayoutObject
Layout( const Column<T>* column,
        const Constraints& cons)
{$FUNC
    assert( column);
    LayoutObject object{ column, column->getWidgets().size()};

    Constraints space_left{ sf::Vector2f(cons) - sf::Vector2f{0, 2 * column->getPadding()}};
    sf::Vector2f position{ 0, column->getPadding()};
    for ( auto& widget : column->getWidgets() )
    {
        $$
        LayoutObject child = Layout( widget, space_left);
        assert( space_left >= child.getSize());
        $$

        sf::Vector2f s = child.getSize();
        sf::Vector2f p = child.getPosition();
        Geometry g{ child.getGeometry()};
        $M( "Child: (%f, %f) (%f, %f)\n", g.tl().x, g.tl().y, g.size().x, g.size().y);
        $M( "Child: (%f, %f) (%f, %f)\n", p.x, p.y, s.x, s.y);

        child.setPosition( position);

        s = child.getSize();
        p = child.getPosition();
        g = child.getGeometry();
        $M( "Child: (%f, %f) (%f, %f)\n", g.tl().x, g.tl().y, g.size().x, g.size().y);
        $M( "Child: (%f, %f) (%f, %f)\n", p.x, p.y, s.x, s.y);

        space_left -= {0, child.getSize().y + column->getPadding()};
        position.y += child.getSize().y + column->getPadding();

        $$
        object.push_back( std::move( child));
        $$
    }

    object.setSize( {cons.width(), position.y});

    $M( "Returning Column<T> (%f, %f) (%f, %f)\n", object.getPosition().x, object.getPosition().y, object.getSize().x, object.getSize().y);
    return object;
}

} // namespace xui
