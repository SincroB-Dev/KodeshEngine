#include "shape.h"

void Shape2D::render()
{
    if (!visible)
    {
        return;
    }

    glPushMatrix();
    
    transform.apply();

    // Aqui vão os seus gráficos OpenGL
    glBegin(mode);
    
        glColor3f(color.r, color.g, color.b); // Cor do objeto;
        
        for (const auto &it : vertices)
        {
            glVertex2f(it.x, it.y);
        }

    glEnd();

    glPopMatrix();
}
