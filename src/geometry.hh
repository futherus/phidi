#pragma once

#include <SFML/System/Vector2.hpp>
#include <assert.h>

class Constraints final
{
private:
    sf::Vector2<float> size_;

public:
    explicit Constraints( sf::Vector2<float> size)
        : size_{ size}
    {}

    explicit Constraints( float width, float height)
        : Constraints{ {width, height}}
    {}

    float width()  const { return size_.x; }
    float height() const { return size_.y; }

    operator sf::Vector2<float>() const { return size_; }

    friend bool
    operator==( const Constraints& lhs, const sf::Vector2<float>& rhs) { return (lhs.size_.x == rhs.x) && (lhs.size_.y == rhs.y); }
    friend bool
    operator!=( const Constraints& lhs, const sf::Vector2<float>& rhs) { return !(lhs == rhs);                                    }
    friend bool
    operator<=( const Constraints& lhs, const sf::Vector2<float>& rhs) { return (lhs.size_.x <= rhs.x) && (lhs.size_.y <= rhs.y); }
    friend bool
    operator>=( const Constraints& lhs, const sf::Vector2<float>& rhs) { return (lhs.size_.x >= rhs.x) && (lhs.size_.y >= rhs.y); }

    friend bool
    operator==( const sf::Vector2<float>& lhs, const Constraints& rhs) { return rhs == lhs; }
    friend bool
    operator!=( const sf::Vector2<float>& lhs, const Constraints& rhs) { return rhs != lhs; }
    friend bool
    operator<=( const sf::Vector2<float>& lhs, const Constraints& rhs) { return rhs >= lhs; }
    friend bool
    operator>=( const sf::Vector2<float>& lhs, const Constraints& rhs) { return rhs <= lhs; }

    Constraints&
    operator+=( const sf::Vector2<float> rhs) { this->size_ += rhs; return *this; }
    Constraints&
    operator-=( const sf::Vector2<float> rhs) { this->size_ -= rhs; return *this; }
    Constraints
    operator+( const sf::Vector2<float> rhs) const { Constraints tmp = *this; return tmp += rhs; }
    Constraints
    operator-( const sf::Vector2<float> rhs) const { Constraints tmp = *this; return tmp -= rhs; }
};

// FIXME: Remove sf::Vector dependency.
class Geometry final
{
private:
    sf::Vector2<float> tl_;
    sf::Vector2<float> sz_;

    void verify() const { assert( sz_.x > 0 && sz_.y > 0); }

public:
    Geometry( const sf::Vector2<float>& tl,
              const sf::Vector2<float>& sz)
        : tl_{ tl}
        , sz_{ sz}
    {
        verify();
    }

    Geometry()
        : tl_{ 0, 0}
        , sz_{ 0, 0}
    {}

    float left()   const { return tl_.x; }
    float right()  const { return tl_.x + sz_.x; }
    float top()    const { return tl_.y; }
    float bottom() const { return tl_.y + sz_.y; }
    float width()  const { return sz_.x; }
    float height() const { return sz_.y; }

    sf::Vector2<float> tl() const { return tl_; }
    sf::Vector2<float> br() const { return tl_ + sz_; }
    sf::Vector2<float> tr() const { return {top(),    right()}; }
    sf::Vector2<float> bl() const { return {bottom(), left()}; }

    sf::Vector2<float> size() const { return sz_; }

    void setLeft  ( float val) { tl_.x = val; verify(); }
    void setTop   ( float val) { tl_.y = val; verify(); }
    void setWidth ( float val) { sz_.x = val; verify(); }
    void setHeight( float val) { sz_.y = val; verify(); }
    void setSize   ( sf::Vector2<float> size) { sz_ = size; verify(); }
    void setTopLeft( sf::Vector2<float> tl  ) { tl_ = tl;   verify(); }

    void translate( sf::Vector2<float> shift) { tl_ += shift; verify(); }

    bool
    contains( sf::Vector2<float> point) const
    {
        return (left() <= point.x && point.x <= right() &&
                top()  <= point.y && point.y <= bottom());
    }
};
