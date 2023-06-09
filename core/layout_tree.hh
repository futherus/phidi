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
#include "../core/dbg_info.hh"

#include "nlohmann/json_fwd.hpp"

using Json = nlohmann::ordered_json;

namespace xui
{

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
        , render_{ []( ObjectImpl* object__,
                       sf::RenderTarget& target__,
                       const Geometry& geometry__ )
                    {
                        auto* tmp = static_cast<const T*>( object__);
                        Render( *tmp, geometry__, target__);
                        drawOutline( target__, geometry__);
                    }}
        , dump_info_{ []( ObjectImpl* object__)
                      {
                          $M( "Call dump_info_ Dump!!!\n");
                          return DumpInfo( *static_cast<const T*>( object__));
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
    Render( const LayoutObject& obj, sf::RenderTarget& target);

    friend Json
    DumpInfo( const LayoutObject& obj, int tree_depth);

    void adjust();

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
    drawOutline( sf::RenderTarget& target, const Geometry& geometry);

private:
    Geometry geometry_;
    Container children_;
    ObjectImpl* widget_;

    RenderCall* render_;
    DumpInfoCall* dump_info_;
};

} // namespace xui


