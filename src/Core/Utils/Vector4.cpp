#include "Core/Utils/Vector4.hpp"

#include <cmath>

namespace core
{
    namespace mathutils
    {
        float Vector4::length() const
        {
            return std::sqrt(x * x + y * y + z * z + w * w);
        }

        float Vector4::dot(const Vector4 &other) const
        {
            return x * other.x + y * other.y + z * other.z + w * other.w;
        }

        Vector4 Vector4::operator-(const Vector4 &other) const
        {
            return Vector4(x - other.x, y - other.y, z - other.z, w - other.w);
        }

        Vector4 Vector4::operator+(const Vector4 &other) const
        {
            return Vector4(x + other.x, y + other.y, z + other.z, w + other.w);
        }

        Vector4 Vector4::operator/(const Vector4 &other) const
        {
            return Vector4(x / other.x, y / other.y, z / other.z, w / other.w);
        }

        Vector4 Vector4::operator/(float scalar) const
        {
            return Vector4(x / scalar, y / scalar, z / scalar, w / scalar); 
        }
        
        Vector4 Vector4::operator*(const Vector4 &other) const
        {
            return Vector4(x * other.x, y * other.y, z * other.z, w * other.w);
        }

        Vector4 Vector4::operator*(float scalar) const
        {
            return Vector4(x * scalar, y * scalar, z * scalar, w * scalar);
        }

        Vector4 operator*(float scalar, const Vector4 &v)
        {
            return v * scalar;
        }

        Vector4 Vector4::operator=(const Vector4 &other)
        {
            x = other.x;
            y = other.y;
            z = other.z;
            w = other.w;
            return *this;
        }
    }
}