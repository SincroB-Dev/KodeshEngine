#include "Core/Maths/Vector3.hpp"

#include <cmath>

namespace core
{
    namespace mathutils
    {
        float Vector3::length() const
        {
            return std::sqrt(x * x + y * y + z * z);
        }

        float Vector3::dot(const Vector3 &other) const
        {
            return x * other.x + y * other.y + z * other.z;
        }

        Vector3 Vector3::operator-(const Vector3& other) const
        {
            return Vector3(x - other.x, y - other.y, z - other.z);
        }

        Vector3 Vector3::operator+(const Vector3& other) const
        {
            return Vector3(x + other.x, y + other.y, z + other.z);
        }

        Vector3 Vector3::operator/(const Vector3 &other) const
        {
            return Vector3(x / other.x, y / other.y, z / other.z);
        }
        
        Vector3 Vector3::operator/(float scalar) const
        {
            return Vector3(x / scalar, y / scalar, z / scalar);
        }

        Vector3 Vector3::operator*(const Vector3 &other) const
        {
            return Vector3(x * other.x, y * other.y, z * other.z);
        }

        Vector3 Vector3::operator*(float scalar) const
        {
            return Vector3(x * scalar, y * scalar, z * scalar);
        }

        Vector3 operator*(float scalar, const Vector3 &v)
        {
            return v * scalar;
        }

        Vector3 Vector3::operator=(const Vector3 &other)
        {
            x = other.x;
            y = other.y;
            z = other.z;
            return *this;
        }
    }
}