#ifndef VEC2_H
#define VEC2_H

#include "proxies.h"
#include <iostream>

namespace maths
{
    class vec2
    {
    public:

        float x, y;

        const Proxy<2> xx, xy, yx, yy;
        
        inline vec2(float x, float y)
            : x(x), y(y), 
            xx(this->x, this->x), xy(this->x, this->y), 
            yx(this->y, this->x), yy(this->y, this->y) {}
        inline vec2(): vec2(0.0f, 0.0f) {}

        float& operator[](int i) { return reinterpret_cast<float*>(this)[i]; }
        const float& operator[](int i) const { return reinterpret_cast<const float*>(this)[i]; }

        vec2 operator*(const vec2 &other) const;
        vec2 operator*(float scalar) const;
        friend vec2 operator*(float scalar, const vec2 &v);

        vec2 operator=(const vec2 &other);

        vec2 operator-(const vec2& other) const;
        vec2 operator+(const vec2& other) const;
        vec2 operator/(const vec2 &other) const;
        vec2 operator/(float scalar) const;

        virtual float length() const;
        virtual float dot(const vec2 &other) const;
        virtual int size() const { return 2; }

        inline vec2& operator+=(const vec2 &other)
        {
            x += other.x;
            y += other.y;
            return *this;
        }

        inline friend std::ostream& operator<<(std::ostream &os, const vec2 &v)
        {
            os << "(" << "x: " << v.x << ", y: " << v.y << ")";
            return os;
        }
    };
};

#endif