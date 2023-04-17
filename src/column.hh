#pragma once

#include "widget.hh"

namespace xui
{

enum class CrossAxisAlignment
{
    Start,
    End,
    Center
};

enum class MainAxisAlignment
{
    Start,
    End,
    Center
};

class LayoutPolicy
{
public:
    MainAxisAlignment main_align_;
    CrossAxisAlignment cross_align_;
};

template<typename T>
class Column final
{
public:
    Column( int padding, LayoutPolicy layout_policy)
        : padding_{ padding}
        , layout_policy_{ layout_policy}
        , widgets_{}
    {}

    Column( const Column&) = delete;
    Column& operator=( const Column&) = delete;
    Column( Column&&) = delete;
    Column& operator=( Column&&) = delete;
    ~Column() = default;

    int getPadding() const { return padding_; }
    LayoutPolicy getLayoutPolicy() const { return layout_policy_; }
    const std::vector<T>& getWidgets() const { return widgets_; }
    std::vector<T>& getWidgets() { return widgets_; }
    void add( T&& widget) { widgets_.push_back( std::move( widget)); }

private:
    int padding_;
    LayoutPolicy layout_policy_;
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
Layout( const Column<T>& column,
        const Constraints& cons)
{$FUNC
    size_t n_widgets = column.getWidgets().size();
    LayoutObject object{ column, n_widgets};

    const float padding = column.getPadding();

    float curr = 0;
    float vspace = cons.height() - n_widgets * padding;
    float xspace = cons.width() - 2 * padding;
    float col_width = 100;

    for ( const T& widget : column.getWidgets() )
    {
        LayoutObject child = Layout( widget, Constraints{ xspace, vspace});

        child.setPosition({ 0, curr});

        vspace -= child.getSize().y;
        curr   += child.getSize().y;

        col_width = std::max( col_width, child.getSize().x);

        object.push_back( std::move( child));
    }
    $M( "vspace = %f\n", vspace);

    const float step = (vspace + n_widgets * padding) / (object.size() + 1);
    int i = 1;

    for ( auto& child : object )
    {
        sf::Vector2f pos = child.getPosition();

        switch ( column.getLayoutPolicy().cross_align_)
        {
            case CrossAxisAlignment::Start:
                pos.x = 0;
                break;
            case CrossAxisAlignment::End:
                pos.x = col_width - child.getSize().x;
                break;
            case CrossAxisAlignment::Center:
                pos.x = (col_width - child.getSize().x) / 2;
                break;
        }
        pos.x += padding;

        pos.y += step * i;
        i++;

        child.setPosition( pos);
    }
    col_width += 2 * padding;

    object.setSize( sf::Vector2f{ col_width, cons.height()});
/*
    Constraints space_left{ sf::Vector2f{cons} - sf::Vector2f{0, 2 * column.getPadding()}};
    sf::Vector2f position{ 0, column.getPadding()};
    for ( const T& widget : column.getWidgets() )
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

        space_left -= {0, child.getSize().y + column.getPadding()};
        position.y += child.getSize().y + column.getPadding();

        $$
        object.push_back( std::move( child));
        $$
    }

    object.setSize( {cons.width(), position.y});
*/
    $M( "Returning Column<T> (%f, %f) (%f, %f)\n", object.getPosition().x, object.getPosition().y, object.getSize().x, object.getSize().y);
    return object;
}

} // namespace xui
