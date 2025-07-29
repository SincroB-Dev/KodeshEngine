#include "transform2d.h"
#include <GL/gl.h>

void Transform2D::apply()
{
    // Transformações da malha.
    glTranslatef(localPosition.x, localPosition.y, 0.0f);
    glRotatef(localAngle, 0.0f, 0.0f, 1.0f);
    glScalef(localScale.x, localScale.y, 0.01f);
}