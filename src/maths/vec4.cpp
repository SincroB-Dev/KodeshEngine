#include "vec4.h"

#include <cmath>

namespace maths
{
    float vec4::length() const
    {
        return std::sqrt(x * x + y * y + z * z + w * w);
    }

    float vec4::dot(const vec4 &other) const
    {
        return x * other.x + y * other.y + z * other.z + w * other.w;
    }

    vec4 vec4::operator-(const vec4 &other) const
    {
        return vec4(x - other.x, y - other.y, z - other.z, w - other.w);
    }

    vec4 vec4::operator+(const vec4 &other) const
    {
        return vec4(x + other.x, y + other.y, z + other.z, w + other.w);
    }

    vec4 vec4::operator/(const vec4 &other) const
    {
        return vec4(x / other.x, y / other.y, z / other.z, w / other.w);
    }

    vec4 vec4::operator/(float scalar) const
    {
        return vec4(x / scalar, y / scalar, z / scalar, w / scalar); 
    }
    
    vec4 vec4::operator*(const vec4 &other) const
    {
        return vec4(x * other.x, y * other.y, z * other.z, w * other.w);
    }

    vec4 vec4::operator*(float scalar) const
    {
        return vec4(x * scalar, y * scalar, z * scalar, w * scalar);
    }

    vec4 operator*(float scalar, const vec4 &v)
    {
        return v * scalar;
    }

    vec4 vec4::operator=(const vec4 &other)
    {
        x = other.x;
        y = other.y;
        z = other.z;
        w = other.w;
    }
};