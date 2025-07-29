#ifndef COLOR_H
#define COLOR_H

#include <iostream>

struct Color
{
    float r=1.0f;
    float g=1.0f; 
    float b=1.0f;
    float a=1.0f;

    Color() = default;
    Color(float r, float g, float b, float a = 1.0f)
        : r(r), g(g), b(b), a(a) {}

    friend std::ostream& operator<<(std::ostream &os, const Color &c)
    {
        os << "red: " << c.r << ", green: " << c.g << ", blue: " << c.b << ", alpha:" << c.a;
        return os;
    }
};

#endif