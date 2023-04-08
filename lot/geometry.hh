#pragma once
#include <cstdint>

struct Size
{
    std::size_t width;
    std::size_t height;

    bool operator==( const Size& rhs) const { return (width == rhs.width) && (height == rhs.height); }
    bool operator!=( const Size& rhs) const { return !(*this == rhs);                                }
    bool operator< ( const Size& rhs) const { return (width <  rhs.width) && (height <  rhs.height); }
    bool operator<=( const Size& rhs) const { return (width <= rhs.width) && (height <= rhs.height); }
    bool operator> ( const Size& rhs) const { return (width >  rhs.width) && (height >  rhs.height); }
    bool operator>=( const Size& rhs) const { return (width >= rhs.width) && (height >= rhs.height); }
};

struct Position
{
    int x, y;

    Position& operator+=(const Position& other)
    {
        x += other.x;
        y += other.y;
        return *this;
    }

    Position operator+(Position other)
    {
        return other += *this;
    }

    Position& operator-=(const Position& other)
    {
        x -= other.x;
        y -= other.y;
        return *this;
    }

    Position operator-(Position other)
    {
        return other -= *this;
    }
};

struct Geometry
{
    Size size;
    Position position;
};

using Constraints = Size;

