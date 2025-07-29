#ifndef POINT_H
#define POINT_H

#include <iostream>

struct Point
{
    float x=0.0f, y=0.0f;

    friend std::ostream& operator<<(std::ostream &os, const Point &p)
    {
        os << "x: " << p.x << ", y: " << p.y;
        return os;
    }

    Point operator+(const Point &other) const
    {
        return {x + other.x, y + other.y};
    }

    Point &operator+=(const Point &other)
    {
        x += other.x;
        y += other.y;
        return *this;
    }
};

#endif