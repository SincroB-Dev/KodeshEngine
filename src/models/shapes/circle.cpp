
#include <GL/gl.h>
#include <cmath>

#include "circle.h"
#include "internal.h"

Shape2DCircle::Shape2DCircle(Transform2D &t, Color &c, int vertices)
    : Shape2D(t, c, GL_TRIANGLE_FAN)
{
    Register();

    for (int v=0; v<=vertices; v++)
    {
        float angle = 2.0f * PI * v / vertices;

        float x = cos(angle) * transform.localScale.x;
        float y = sin(angle) * transform.localScale.y;

        this->vertices.push_back(maths::vec2(x, y));
    }
}

Shape2DCircle::Shape2DCircle(Transform2D &t, Color &c): Shape2DCircle(t, c, 32)
{
    
}
