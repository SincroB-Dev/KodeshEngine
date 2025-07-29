#ifndef VEC2_H
#define VEC2_H

#include "proxies.h"

namespace maths
{
    class vec2
    {
    public:

        float x, y;

        const Proxy<2> xx, xy, yx, yy;
        
        inline vec2(float x = 0.0f, float y = 0.0f)
            : x(x), y(y), 
            xx(this->x, this->x), xy(this->x, this->y), 
            yx(this->y, this->x), yy(this->y, this->y) {}

        float& operator[](int i) { return reinterpret_cast<float*>(this)[i]; }
        const float& operator[](int i) const { return reinterpret_cast<const float*>(this)[i]; }
    };
}

#endif