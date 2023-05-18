#pragma once

#include <cstdio>
#include <utility>
#include <memory>
#include <vector>
#include <array>
#include <SFML/Graphics.hpp>

#include "../core/graphviz.hh"
#include "../core/geometry.hh"
#include "../core/debug.hh"

#include "nlohmann/json.hpp"

using Json = nlohmann::ordered_json;

namespace xui
{

template <typename T>
Json
DumpInfo( const T& object)
{
    Json info{};
    info["type"] = typeid( T).name();
    return info;
}

class LayoutObject final
{
public:
    using Container = std::vector<LayoutObject>;

    using iterator       = Container::iterator;
    using const_iterator = Container::const_iterator;

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
    LayoutObject( const LayoutObject& other) = delete;
    LayoutObject& operator=( const LayoutObject& other) = delete;

    LayoutObject( LayoutObject&& other) = default;
    LayoutObject& operator=( LayoutObject&& other) = default;

    ~LayoutObject() = default;

private:
    using ObjectImpl = const void;
    using RenderCall = void ( ObjectImpl*, sf::RenderTarget&, const Geometry&);
    using DumpInfoCall = Json ( ObjectImpl*);

public:
    template <typename T>
    LayoutObject( const T& widget,
                  const Geometry& geometry = {},
                  std::size_t prealloc = 0)
        : geometry_{ geometry}
        , children_{}
        , widget_{ std::addressof( widget)}
        , render_{ []( ObjectImpl* object,
                       sf::RenderTarget& target,
                       const Geometry& geometry )
                    {
                        auto* tmp = static_cast<const T*>( object);
                        Render( *tmp, geometry, target);
                        drawOutline( target, geometry);
                    }} /* RenderCall */
        , dump_info_{ []( ObjectImpl* object)
                      {
                          $M( "Call dump_info_ Dump!!!\n");
                          return DumpInfo( *static_cast<const T*>( object));
                      }} /* DumpInfoCall */
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

    friend Json
    DumpInfo( const LayoutObject& obj,
              int tree_depth = 0)
    {
        Json info = obj.dump_info_( obj.widget_);

        if ( tree_depth )
        {
            Json child_info{};
            for ( const auto& child : obj.children_ )
            {
                child_info.emplace_back( DumpInfo( child, tree_depth - 1));
            }

            info["children"] = std::move( child_info);
        }

        return info;
    }

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

    void showGraph( FILE* file) const;

    sf::Vector2f getSize() const { return geometry_.size(); }
    void setSize( const sf::Vector2f& size) { geometry_.setSize( size); }

    sf::Vector2f getPosition() const { return geometry_.tl(); }
    void setPosition( const sf::Vector2f& position) { geometry_.setTopLeft( position); }

    Geometry getGeometry() const { return geometry_; }
    void setGeometry( const Geometry& geometry) { geometry_ = geometry; }

    ObjectImpl* getWidget() const { return widget_; }

private:
    static void
    drawOutline( sf::RenderTarget& target,
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
    Container children_;
    ObjectImpl* widget_;

    RenderCall* render_;
    DumpInfoCall* dump_info_;
};

} // namespace xui


