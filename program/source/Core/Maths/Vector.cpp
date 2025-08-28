#include "Core/Maths/Vector.hpp"

#include <cmath>

namespace core
{
    namespace mathutils
    {
        float Vector::length() const
        {
            return std::sqrt(x * x + y * y);
        }

        float Vector::dot(const Vector &other) const
        {
            return x * other.x + y * other.y;
        }

        Vector Vector::operator-(const Vector& other) const
        {
            return Vector(x - other.x, y - other.y);
        }

        Vector Vector::operator+(const Vector& other) const
        {
            return Vector(x + other.x, y + other.y);
        }

        Vector Vector::operator/(const Vector &other) const
        {
            return Vector(x / other.x, y / other.y);
        }

        Vector Vector::operator/(float scalar) const
        {
            return Vector(x / scalar, y / scalar);
        }

        Vector Vector::operator*(const Vector &other) const
        {
            return Vector(x * other.x, y * other.y);
        }

        Vector Vector::operator*(float scalar) const
        {
            return Vector(x * scalar, y * scalar);
        }

        Vector operator*(float scalar, const Vector &v)
        {
            return v * scalar;
        }

        Vector Vector::operator=(const Vector &other)
        {
            x = other.x;
            y = other.y;
            return *this;
        }
    }
}