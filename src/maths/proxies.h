#ifndef PROXIES_H
#define PROXIES_H

namespace maths
{
    struct vec2;
    struct vec3;
    struct vec4;

    template <int N> struct Proxy;

    template<> struct Proxy<2>
    {
        const float &x;
        const float &y;

        inline Proxy(const float &x, const float &y): x(x), y(y) {}

        operator struct vec2() const;
    };

    template<> struct Proxy<3>
    {
        const float &x;
        const float &y;
        const float &z;

        inline Proxy(const float &x, const float &y, const float &z): x(x), y(y), z(z) {}

        operator struct vec3() const;
    };

    template<> struct Proxy<4>
    {
        const float &x;
        const float &y;
        const float &z;
        const float &w;

        inline Proxy(const float &x, const float &y, const float &z, const float &w): x(x), y(y), z(z), w(w) {}

        operator struct vec4() const;
    };
}

#endif