#include "vec2.h"

#include <cmath>

namespace maths
{
    float vec2::length() const
    {
        return std::sqrt(x * x + y * y);
    }

    float vec2::dot(const vec2 &other) const
    {
        return x * other.x + y * other.y;
    }

    vec2 vec2::operator-(const vec2& other) const
    {
        return vec2(x - other.x, y - other.y);
    }

    vec2 vec2::operator+(const vec2& other) const
    {
        return vec2(x + other.x, y + other.y);
    }

    vec2 vec2::operator/(const vec2 &other) const
    {
        return vec2(x / other.x, y / other.y);
    }

    vec2 vec2::operator/(float scalar) const
    {
        return vec2(x / scalar, y / scalar);
    }

    vec2 vec2::operator*(const vec2 &other) const
    {
        return vec2(x * other.x, y * other.y);
    }

    vec2 vec2::operator*(float scalar) const
    {
        return vec2(x * scalar, y * scalar);
    }

    vec2 operator*(float scalar, const vec2 &v)
    {
        return v * scalar;
    }

    vec2 vec2::operator=(const vec2 &other)
    {
        x = other.x;
        y = other.y;
        return *this;
    }
};