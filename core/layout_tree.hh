#pragma once

#include <cstdio>
#include <utility>
#include <memory>
#include <vector>
#include <array>
#include <SFML/Graphics.hpp>

#include "core/graphviz.hh"
#include "core/geometry.hh"
#include "core/debug.hh"

namespace xui
{

class LayoutObject final
{
public:
    using container = std::vector<LayoutObject>;

    using iterator = container::iterator;
    using const_iterator = container::const_iterator;

    iterator            begin()       { return children_.begin(); }
    const_iterator      begin() const { return children_.begin(); }
    iterator              end()       { return children_.end();   }
    const_iterator        end() const { return children_.end();   }

    size_t               size() const { return children_.size();  }
    bool                empty() const { return children_.empty(); }
          LayoutObject& front()       { return children_.front(); }
    const LayoutObject& front() const { return children_.front(); }
          LayoutObject&  back()       { return children_.back();  }
    const LayoutObject&  back() const { return children_.back();  }

    void push_back( LayoutObject&& value) { children_.push_back( std::move( value)); }

public:
    using This = const void*;
    using RenderOp = void ( This, sf::RenderTarget&, const Geometry&);

public:
    LayoutObject( const LayoutObject& other) = delete;
    LayoutObject& operator=( const LayoutObject& other) = delete;

    LayoutObject( LayoutObject&& other) = default;
    LayoutObject& operator=( LayoutObject&& other) = default;

    ~LayoutObject() = default;

    template <typename T>
    LayoutObject( const T& widget,
                  const Geometry& geometry = {},
                  std::size_t prealloc = 0)
        : geometry_{ geometry}
        , widget_{ std::addressof( widget)}
        , children_{}
        , render_{ []( This wgt, sf::RenderTarget& target, const Geometry& geom)
                   {
                       //   fprintf( stderr, "[RENDER]: %10s s: (%4f %4f) p: (%4f %4f)\n", typeid( T).name(),
                       //            geom.size().x, geom.size().y, geom.tl().x, geom.tl().y);

                       auto* tmp = static_cast<const T*>( wgt);
                       Render( *tmp, geom, target);

                       dbgRenderGeometry( target, geom);
                   }}
    {
        children_.reserve( prealloc);
    }

    template <typename T>
    explicit LayoutObject( const T& widget,
                           std::size_t prealloc)
        : LayoutObject{ widget, Geometry{}, prealloc}
    {}

    friend void
    Render( const LayoutObject& obj,
            sf::RenderTarget& target)
    {
        obj.render_( obj.widget_, target, obj.geometry_);
        for ( auto& child : obj.children_ )
        {
            Render( child, target);
        }
    }

public:
    void
    adjust()
    {
        for ( auto& child : children_ )
        {
            child.geometry_.translate( geometry_.tl());
            child.adjust();
        }
    }

    template <typename OutputIt>
    void
    find( sf::Vector2<float> point,
          OutputIt it) const
    {
        if ( geometry_.contains( point) )
        {
            it = this;

            for ( auto& child : children_ )
            {
                // Double check to reduce recursive calls
                if ( child.geometry_.contains( point) )
                {
                    child.find( point, it);
                }
            }
        }
    }

    sf::Vector2f getSize() const { return geometry_.size(); }
    void setSize( const sf::Vector2f& size) { geometry_.setSize( size); }

    sf::Vector2f getPosition() const { return geometry_.tl(); }
    void setPosition( const sf::Vector2f& position) { geometry_.setTopLeft( position); }

    Geometry getGeometry() const { return geometry_; }
    void setGeometry( const Geometry& geometry) { geometry_ = geometry; }

    const void* getWidget() const { return widget_; }

private:
    static void
    dbgRenderGeometry( sf::RenderTarget& target,
                       const Geometry& geometry)
    {
        sf::RectangleShape rectangle;
        rectangle.setSize( sf::Vector2f{ geometry.size().x, geometry.size().y});
        rectangle.setFillColor( sf::Color::Transparent);
        rectangle.setOutlineColor( sf::Color::Red);
        rectangle.setOutlineThickness( 1);
        rectangle.setPosition( geometry.tl().x, geometry.tl().y);
        target.draw( rectangle);
    }

private:
    Geometry geometry_;
    This widget_;
    container children_;

    RenderOp* render_;
};

} // namespace xui
