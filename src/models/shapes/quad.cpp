#include <GL/gl.h>
#include "quad.h"

Shape2DQuad::Shape2DQuad(Transform2D &t, Color &c): Shape2D(t, c) 
{
    Register();

    vertices.push_back(maths::vec2(-transform.localScale.x, -transform.localScale.y));
    vertices.push_back(maths::vec2( transform.localScale.x, -transform.localScale.y));
    vertices.push_back(maths::vec2( transform.localScale.x,  transform.localScale.y));
    vertices.push_back(maths::vec2(-transform.localScale.x,  transform.localScale.y));
}