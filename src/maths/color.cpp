#include "color.h"

#include <GL/gl.h>

Color::Color(float r, float g, float b, float a)
    : r(r), g(g), b(b), a(a) 
{}

void Color::apply()
{
    glColor4f(r, g, b, a);
}