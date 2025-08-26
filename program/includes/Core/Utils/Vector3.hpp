#pragma once

#include <iostream>

#include "Core/Utils/Vector.hpp"

namespace core
{
    namespace mathutils
    {
        class Vector3: public Vector
        {
        public:
            float z;

            inline Vector3(): Vector3(0.0f, 0.0f, 0.0f) {}
            inline Vector3(float x, float y, float z): Vector(x, y), z(z) {}

            Vector3 operator*(const Vector3 &other) const;
            Vector3 operator*(float scalar) const;
            friend Vector3 operator*(float scalar, const Vector3 &v);
            
            Vector3 operator=(const Vector3 &other);
            
            Vector3 operator-(const Vector3 &other) const;
            Vector3 operator+(const Vector3 &other) const;
            Vector3 operator/(const Vector3 &other) const;
            Vector3 operator/(float scalar) const;

            using Vector::dot;

            virtual float length() const override;
            virtual float dot(const Vector3 &other) const;
            virtual int size() const { return 3; }

            inline Vector3& operator+=(const Vector3 &other)
            {
                x += other.x;
                y += other.y;
                z += other.z;
                return *this;
            }

            inline friend std::ostream& operator<<(std::ostream &os, const Vector3 &v)
            {
                os << "<Vector: " << v.x << ", " << v.y << ", " << v.z << ">";
                return os;
            }
        };
    }
}