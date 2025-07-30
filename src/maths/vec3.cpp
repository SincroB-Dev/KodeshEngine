#include "vec3.h"

#include <cmath>

namespace maths
{
    float vec3::length() const
    {
        return std::sqrt(x * x + y * y + z * z);
    }

    float vec3::dot(const vec3 &other) const
    {
        return x * other.x + y * other.y + z * other.z;
    }

    vec3 vec3::operator-(const vec3& other) const
    {
        return vec3(x - other.x, y - other.y, z - other.z);
    }

    vec3 vec3::operator*(const vec3 &other) const
    {
        return vec3(x * other.x, y * other.y, z * other.z);
    }

    vec3 vec3::operator*(float scalar) const
    {
        return vec3(x * scalar, y * scalar, z * scalar);
    }

    vec3 operator*(float scalar, const vec3 &v)
    {
        return v * scalar;
    }

    vec3 vec3::operator=(const vec3 &other)
    {
        x = other.x;
        y = other.y;
        z = other.z;
    }
};