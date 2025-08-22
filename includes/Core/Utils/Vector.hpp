#pragma once

#include <iostream>

namespace core
{
    namespace mathutils
    {
        class Vector
        {
        public:

            float x, y;
            
            inline Vector(): Vector(0.0f, 0.0f) {}
            inline Vector(float x, float y) : x(x), y(y) {}

            float& operator[](int i) { return reinterpret_cast<float*>(this)[i]; }
            const float& operator[](int i) const { return reinterpret_cast<const float*>(this)[i]; }

            Vector operator*(const Vector &other) const;
            Vector operator*(float scalar) const;
            friend Vector operator*(float scalar, const Vector &v);

            Vector operator=(const Vector &other);

            Vector operator-(const Vector& other) const;
            Vector operator+(const Vector& other) const;
            Vector operator/(const Vector &other) const;
            Vector operator/(float scalar) const;

            virtual float length() const;
            virtual float dot(const Vector &other) const;
            virtual int size() const { return 2; }

            inline Vector& operator+=(const Vector &other)
            {
                x += other.x;
                y += other.y;
                return *this;
            }

            inline friend std::ostream& operator<<(std::ostream &os, const Vector &v)
            {
                os << "<Vector: " << v.x << ", " << v.y << ">";
                return os;
            }
        };
    }
}
