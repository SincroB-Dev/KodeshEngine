#ifndef VEC4_H
#define VEC4_H

#include <iostream>

#include "proxies.h"
#include "vec3.h"

namespace maths
{
    class vec4: public vec3
    {
    public:
        float w;

        const Proxy<3> zxw, wzx, zwx;
        const Proxy<4> xyzw;

        inline vec4(float x, float y, float z, float w)
            : vec3(x, y, z), w(w),
            zxw(this->z, this->x, this->y), wzx(this->w, this->x, this->z), zwx(this->z, this->w, this->x),
            xyzw(this->x, this->y, this->z, this->w) {}
        inline vec4(): vec4(0.0f, 0.0f, 0.0f, 0.0f) {}
    
        vec4 operator*(const vec4 &other) const;
        vec4 operator*(float scalar) const;
        friend vec4 operator*(float scalar, const vec4 &v);
        
        vec4 operator=(const vec4 &other);

        vec4 operator-(const vec4 &other) const;
        vec4 operator+(const vec4 &other) const;
        vec4 operator/(const vec4 &other) const;
        vec4 operator/(float scalar) const;

        virtual float length() const override;
        virtual float dot(const vec4 &other) const;
        virtual int size() const { return 4; }
    };
};

#endif