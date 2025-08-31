#pragma once

#include <iostream>

#include <Core/Maths/Vector3.hpp>

namespace core
{
    namespace mathutils
    {
        class Vector4: public Vector3
        {
        public:
            float w;

            inline Vector4(): Vector4(0.0f, 0.0f, 0.0f, 0.0f) {}
            inline Vector4(float x, float y, float z, float w): Vector3(x, y, z), w(w) {}
        
            Vector4 operator*(const Vector4 &other) const;
            Vector4 operator*(float scalar) const;
            friend Vector4 operator*(float scalar, const Vector4 &v);
            
            Vector4 operator=(const Vector4 &other);

            Vector4 operator-(const Vector4 &other) const;
            Vector4 operator+(const Vector4 &other) const;
            Vector4 operator/(const Vector4 &other) const;
            Vector4 operator/(float scalar) const;

            virtual float length() const override;

            using Vector::dot;
            using Vector3::dot;

            virtual float dot(const Vector4 &other) const;
            virtual int size() const { return 4; }

            inline Vector4& operator+=(const Vector4 &other)
            {
                x += other.x;
                y += other.y;
                z += other.z;
                w += other.w;
                return *this;
            }

            inline friend std::ostream& operator<<(std::ostream &os, const Vector4 &v)
            {
                os << "(" << "x: " << v.x << ", y: " << v.y << ", z: " << v.z << ", w:" << v.w << ")";
                return os;
            }
        };
    }
}