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
    };
};

#endif