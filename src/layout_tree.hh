#pragma once

#include <cstdio>
#include <utility>
#include <memory>
#include <vector>
#include <array>
#include <SFML/Graphics.hpp>

#include "geometry.hh"
#include "debug.hh"

namespace xui
{

class LayoutObject final
{
public:
    using ObjectList = std::vector<LayoutObject>;

    using iterator = ObjectList::iterator;
    using const_iterator = ObjectList::const_iterator;

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

    void push_back(       LayoutObject&& value) { children_.push_back( std::move( value)); }
    //void push_back( const LayoutObject&  value) { children_.push_back( value);             }

public:
    sf::Vector2f getSize() const { return geometry_.size(); }
    void setSize( const sf::Vector2f& size) { geometry_.setSize( size); }

    sf::Vector2f getPosition() const { return geometry_.tl(); }
    void setPosition( const sf::Vector2f& position) { geometry_.setTopLeft( position); }

    Geometry getGeometry() const { return geometry_; }
    void setGeometry( const Geometry& geometry) { geometry_ = geometry; }

public:
    LayoutObject( LayoutObject&& other) = default;
    LayoutObject& operator=( LayoutObject&& other) = default;

private:
    struct LayoutObjectConcept
    {
        virtual ~LayoutObjectConcept() = default;
        virtual void render( sf::RenderTarget& rt, const Geometry& geometry) const = 0;
    };

    template <typename T>
    class LayoutObjectModel final
        : public LayoutObjectConcept
    {
    public:
        LayoutObjectModel( T widget)
            : widget_{ widget}
        {}

        void render( sf::RenderTarget& target, const Geometry& geometry) const override
        {
            fprintf( stderr, "[RENDER]: %10s s: (%4f %4f) p: (%4f %4f)\n", typeid( T).name(),
                     geometry.size().x, geometry.size().y, geometry.tl().x, geometry.tl().y);

            Render( widget_, geometry, target);

            sf::RectangleShape rectangle;
            rectangle.setSize( sf::Vector2f{ geometry.size().x, geometry.size().y});
            rectangle.setFillColor( sf::Color::Transparent);
            rectangle.setOutlineColor( sf::Color::Red);
            rectangle.setOutlineThickness( 1);
            rectangle.setPosition( geometry.tl().x, geometry.tl().y);
            target.draw( rectangle);
        }

    private:
        T widget_;
    };

public:
    using Concept = LayoutObjectConcept;

    template <typename T>
    explicit LayoutObject( const T& widget, const Geometry& geometry = {}, std::size_t prealloc = 0)
        : impl_buffer_{}
        , children_{}
        , geometry_{ geometry}
    {
        children_.reserve( prealloc);
        using Model = LayoutObjectModel<T>;
        static_assert( sizeof( *this) <= 64, "Cache line size drop!!!");
        static_assert( sizeof( Model) == sizeof( impl_buffer_), "Size of Model exceed sizeof(impl_buffer_)!" );
        ::new( impl()) Model{ widget};
    }

    template <typename T>
    explicit LayoutObject( const T& widget, std::size_t prealloc)
        : LayoutObject{ widget, Geometry{}, prealloc}
    {}

    friend void
    Adjust( LayoutObject& obj)
    {
        for ( auto& child : obj.children_ )
        {
            child.geometry_.translate( obj.geometry_.tl());
            Adjust( child);
        }
    }

    friend void
    Render( const LayoutObject& obj, sf::RenderTarget& target)
    {
        obj.impl()->render( target, obj.geometry_);
        for ( auto& child : obj.children_ )
        {
            Render( child, target);
        }
    }

private:
          Concept* impl()       { return reinterpret_cast<      Concept*>( impl_buffer_.data()); }
    const Concept* impl() const { return reinterpret_cast<const Concept*>( impl_buffer_.data()); }

    // std::array<char, sizeof( LayoutObjectModel<LayoutObject>)> impl_buffer_;
    //!!!!
    //!!!!
    std::array<char, 16> impl_buffer_;
    //!!!!
    //!!!!

    std::vector<LayoutObject> children_;
    Geometry geometry_;
};

} // namespace xui
