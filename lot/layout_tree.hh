#pragma once

#include <cstdio>
#include <utility>
#include <memory>
#include <vector>
#include <list>
#include <stack>
#include <array>
#include <algorithm>
#include <SFML/Graphics.hpp>

#include "geometry.hh"

namespace lot {

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
    const Size& getSize() const { return geometry_.size; }
    void setSize( const Size& size) { geometry_.size = size; }

    const Position& getPosition() const { return geometry_.position; }
    void setPosition( const Position& position) { geometry_.position = position; }

    const Geometry& getGeometry() const { return geometry_; }
    void setGeometry( const Geometry& geometry) { geometry_ = geometry; }

public:
#if !defined(IM_KNOW_WHAT_IM_DOING)
    LayoutObject( const LayoutObject& other) = delete;
    LayoutObject& operator=( const LayoutObject& other) = delete;
#endif

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
        LayoutObjectModel( const T& widget)
            : widget_{ std::addressof( widget)}
        {}

        void render( sf::RenderTarget& target, const Geometry& geometry) const override
        {
            fprintf( stderr, "[RENDER]: %10s s: (%4lu %4lu) p: (%4d %4d)\n", typeid( T).name(),
                     geometry.size.width, geometry.size.height, geometry.position.x, geometry.position.y);

            Render( *widget_, geometry, target);

            sf::RectangleShape rectangle;
            rectangle.setSize(sf::Vector2f(geometry.size.width + 2, geometry.size.height + 2));
            rectangle.setFillColor(sf::Color::Transparent);
            rectangle.setOutlineColor(sf::Color::Red);
            rectangle.setOutlineThickness(1);
            rectangle.setPosition(geometry.position.x - 1, geometry.position.y - 1);
            target.draw(rectangle);
        }

    private:
        const T* widget_;
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
    explicit LayoutObject( const T& widget, const Size& size, const Position& position = {})
        : LayoutObject{ widget, Geometry{ size, position}}
    {}

    template <typename T>
    explicit LayoutObject( const T& widget, std::size_t prealloc)
        : LayoutObject{ widget, Geometry{}, prealloc}
    {}

    void layout()
    {
        for ( auto& child : children_ )
        {
            child.geometry_.position += geometry_.position;
            child.layout();
        }
    }

    friend void Render( const LayoutObject& obj, sf::RenderTarget& target)
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

    std::array<char, sizeof( LayoutObjectModel<LayoutObject> )> impl_buffer_;

    std::vector<LayoutObject> children_;
    Geometry geometry_;
};

} // namespace lot

#if defined(LEGACY)
class LayoutObject
{
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
        LayoutObjectModel( T& widget)
            : widget_{ std::addressof( widget)}
        {}

        void render( sf::RenderTarget& target, const Geometry& geometry) const override
        {
            fprintf( stderr, "[RENDER]: %10s s: (%4lu %4lu) p: (%4d %4d)\n", typeid( T).name(),
                     geometry.size.width, geometry.size.height, geometry.position.x, geometry.position.y);

            Render( *widget_, geometry, target);

            sf::RectangleShape rectangle;
            rectangle.setSize(sf::Vector2f(geometry.size.width, geometry.size.height));
            rectangle.setFillColor(sf::Color::Transparent);
            rectangle.setOutlineColor(sf::Color::Red);
            rectangle.setOutlineThickness(1);
            rectangle.setPosition(geometry.position.x, geometry.position.y);
            target.draw(rectangle);
        }

    private:
        const T* widget_;
    };

public:
    using Object = LayoutObjectConcept;

    template <typename T>
    explicit LayoutObject( T& widget, const Geometry& geometry)
        : impl_{ std::make_unique<LayoutObjectModel<T>>( widget)}
        , children_{}
        , geometry{ std::move( geometry)}
    {}

    template <typename T>
    explicit LayoutObject( T& widget, const Size& size = {}, const Position& position = {}) : LayoutObject( widget, Geometry{ size, position})
    {}

    LayoutObject( LayoutObject&& other)
        : impl_{}
        , children_{}
    {
        *this = std::move( other);
    }

    LayoutObject( const LayoutObject& other) = delete;
    LayoutObject& operator=( const LayoutObject& other) = delete;

    LayoutObject& operator=( LayoutObject&& other)
    {
        std::swap( geometry, other.geometry);
        std::swap( impl_, other.impl_);
        std::swap( children_, other.children_);
        return *this;
    }

    friend void Render( const LayoutObject& obj, sf::RenderTarget& target)
    {
        obj.impl_->render( target, obj.geometry);

        for ( auto& child : obj.children_ )
        {
            Render( child, target);
        }
    }

public:
    std::unique_ptr<Object> impl_;
    std::vector<LayoutObject> children_;
    Geometry geometry;
};

#endif



