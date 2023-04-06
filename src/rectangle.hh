#ifndef RECTANGLE_H
#define RECTANGLE_H

#include <SFML/Graphics.hpp>
#include <assert.h>

class Rectangle
{
private:
    sf::Vector2<float> tl_;
    sf::Vector2<float> sz_;

    void verify() const
    {
        assert(sz_.x > 0 && sz_.y > 0);
    }

public:
    Rectangle(const sf::Vector2<float>& tl, const sf::Vector2<float>& sz)
        : tl_(tl), sz_(sz)
    {
        verify();
    }

    Rectangle()
        : tl_(0, 0), sz_(0, 0)
    {}

    Rectangle& operator=(const Rectangle&) = default;
    Rectangle(const Rectangle&) = default;
    virtual ~Rectangle() = default;

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

    void setLeft(float val)   { tl_.x = val; verify(); }
    void setTop(float val)    { tl_.y = val; verify(); }
    void setWidth(float val)  { sz_.x = val; verify(); }
    void setHeight(float val) { sz_.y = val; verify(); }
    void setSize(sf::Vector2<float> size) { sz_ = size; verify(); }
    void setTopLeft(sf::Vector2<float> tl) { tl_ = tl; verify(); }

    bool contains(sf::Vector2<float> point) const
    {
        return (left() <= point.x && point.x <= right() &&
                top()  <= point.y && point.y <= bottom());
    }
};

#endif // RECTANGLE_H