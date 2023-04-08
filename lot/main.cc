#include <SFML/Graphics.hpp>
#include "layout_tree.hh"
#include <cassert>

class Widget final
{
private:
    struct WidgetConcept
    {
        virtual ~WidgetConcept() = default;
        virtual std::unique_ptr<WidgetConcept> clone() const = 0;
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

        virtual std::unique_ptr<WidgetConcept> clone() const
        {
            return std::make_unique<WidgetModel<T>>( *this);
        }

        virtual lot::LayoutObject layout( const Constraints& cons) const
        {
            return Layout( widget_, cons);
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

    Widget( Widget&& other) = default;
    Widget& operator=( Widget&& other) = default;

    Widget( const Widget& other)
        : impl_{ other.impl_->clone()}
    {}

    Widget& operator=( const Widget& other)
    {
        other.impl_->clone().swap( impl_);
        return *this;
    }

    friend lot::LayoutObject
    Layout( const Widget& widget, const Constraints& cons)
    {
        return widget.impl_->layout( cons);
    }

private:
    std::unique_ptr<Concept> impl_;
};

struct Button
{
    Size size;
};

struct Column
{
    int padding;
    std::vector<Widget> widgets;
};

struct Control
{
    Button btn1;
    Button btn2;
};

void
Render( const Button& button,
        const Geometry& geometry,
        sf::RenderTarget& target)
{
    sf::RectangleShape rectangle;
    rectangle.setSize(sf::Vector2f(geometry.size.width, geometry.size.height));
    rectangle.setFillColor(sf::Color::Yellow);
    rectangle.setPosition(geometry.position.x, geometry.position.y);
    target.draw(rectangle);
}

void
Render( const Column& column,
        const Geometry& geometry,
        sf::RenderTarget& target)
{
}

void
Render( const Control& control,
        const Geometry& geometry,
        sf::RenderTarget& target)
{
}

lot::LayoutObject
Layout( const Button& button,
        const Constraints& cons)
{
    assert( cons > button.size );
    return lot::LayoutObject{ button, button.size};
}


lot::LayoutObject
Layout( const Column& column,
        const Constraints& cons)
{
    lot::LayoutObject object{ column, column.widgets.size()};

    Constraints space_left = cons;
    Position position = {0, 10};
    for ( auto& button : column.widgets )
    {
        lot::LayoutObject child = Layout( button, space_left);
        child.setPosition( position);

        assert( space_left.height >= child.getSize().height );

        space_left.height -= (child.getSize().height + column.padding);
        position.y += child.getSize().height + column.padding;

        object.push_back( std::move( child));
    }

    object.setSize( {cons.width, cons.height - space_left.height});
    return object;
}

lot::LayoutObject
Layout( const Control& control,
        const Constraints& cons)
{
    lot::LayoutObject obj{ control};

    Constraints cell{ cons.width, cons.height / 2};
    lot::LayoutObject obj1 = Layout( control.btn1, cell);
    lot::LayoutObject obj2 = Layout( control.btn2, cell);

    obj1.setPosition( {0, 0});
    obj2.setPosition( {0, (int)obj1.getSize().height});

    obj.push_back( std::move( obj1));
    obj.push_back( std::move( obj2));
    return obj;
}

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 800),
                            "My window name", sf::Style::Default);

    sf::View view(sf::FloatRect(0.f, 0.f, 600.f, 600.f));
    view.setViewport(sf::FloatRect(0.f, 0.f, 1.f, 1.f));
    view.setSize(window.getSize().x, window.getSize().y);
    //view.setViewport(sf::FloatRect(0.f, 0.f, 1.f, 1.f));
    window.setView(view);

    Column app;
    app.padding = 10;
    app.widgets.push_back( Button{40, 10});
    app.widgets.push_back( Button{100, 20});
    app.widgets.push_back( Button{100, 20});
    app.widgets.push_back( Button{100, 10});
    app.widgets.push_back( app);

    lot::LayoutObject obj = Layout( app, {600, 600});

    std::fprintf(stderr, "sizeof(lot) = %ld\n", sizeof(lot::LayoutObject));
    while ( window.isOpen() )
    {
        // Event processing
        sf::Event event{};
        while ( window.pollEvent( event) )
        {
            // Request for closing the window
            if ( event.type == sf::Event::Closed )
            {
                window.close();
            } else if ( event.type == sf::Event::Resized )
            {
                sf::FloatRect visibleArea(0.f, 0.f, event.size.width, event.size.height);
                window.setView(sf::View(visibleArea));
                obj = Layout( app, {event.size.width / 2, event.size.height});
                obj.layout();
            }

            if ( event.type == sf::Event::Closed )
            {
                window.close();
            }
        }

        // Clear the whole window before rendering a new frame
        window.clear();

        Render( obj, window);

        // End the current frame and display its contents on screen
        window.display();
    }

    return 0;
}



