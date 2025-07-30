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
    };
}

#endif