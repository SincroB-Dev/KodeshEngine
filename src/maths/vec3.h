#ifndef VEC3_H
#define VEC3_H

#include <iostream>

#include "vec2.h"
#include "proxies.h"

namespace maths
{
    class vec3: public vec2
    {
    public:
        float z;

        const Proxy<2> xz, yz, zx, zy;
        const Proxy<3> xyz, zyx, xzy, yzx;

        inline vec3(float x, float y, float z)
            : vec2(x, y), z(z),
            xz(this->x, this->z), yz(this->y, this->z), zx(this->z, this->x), zy(this->z, this->y),
            xyz(this->x, this->y, this->z), zyx(this->z, this->y, this->x), xzy(this->x, this->z, this->y),
            yzx(this->y, this->z, this->x) {}
        inline vec3(): vec3(0.0f, 0.0f, 0.0f) {}

        vec3 operator*(const vec3 &other) const;
        vec3 operator*(float scalar) const;
        friend vec3 operator*(float scalar, const vec3 &v);
        
        vec3 operator=(const vec3 &other);
        
        vec3 operator-(const vec3 &other) const;
        vec3 operator+(const vec3 &other) const;
        vec3 operator/(const vec3 &other) const;
        vec3 operator/(float scalar) const;

        virtual float length() const override;
        virtual float dot(const vec3 &other) const;
        virtual int size() const { return 3; }

        inline friend std::ostream& operator<<(std::ostream &os, const vec3 &v)
        {
            os << "(" << "x: " << v.x << ", y: " << v.y << ", z: " << v.z << ")";
            return os;
        }
    };
}

#endif