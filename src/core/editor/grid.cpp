#include "grid.h"

#include <GL/gl.h>

namespace core
{
    namespace editor
    {
        void GridView::Draw()
        {
            glBegin(GL_LINES);

            for (int lp=0; lp<=grid_size; lp++)
            {
                if (lp > 0)
                {
                    grid_color.apply();
                }

                if (lp == 0)
                {
                    x_axis_color.apply();
                }

                // Linhas no Eixo X - Positivas
                glVertex2f(lp, -grid_size);
                glVertex2f(lp,  grid_size);

                if (lp == 0)
                {
                    y_axis_color.apply();
                }

                // Linhas no Eixo Y - Positivas
                glVertex2f(-grid_size, lp);
                glVertex2f( grid_size, lp);

                if (lp > 0)
                {
                    // Linhas no Eixo X - Negativas
                    glVertex2f(-lp, -grid_size);
                    glVertex2f(-lp,  grid_size);

                    // Linhas no Eixo Y - Negativas
                    glVertex2f(-grid_size, -lp);
                    glVertex2f( grid_size, -lp);
                }
            }

            glEnd();
        }
    };
};