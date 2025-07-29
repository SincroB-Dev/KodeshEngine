#include <cmath>

#include "star.h"
#include "internal.h"

Shape2DStar::Shape2DStar(Transform2D &t, Color &c, float Re, float Ri, int points)
    : Shape2D(t, c, GL_TRIANGLES)
{
    Register();

    using maths::vec2;

    vec2 center(0.0f, 0.0f); // Centro da estrela
    float angleStep = PI / points;

    std::vector<vec2> outer;

    // Gera os vértices alternando externo e interno
    for (int i = 0; i < points * 2; i++)
    {
        float angle = i * angleStep;
        float radius = (i % 2 == 0) ? Re : Ri;

        float x = cos(angle) * radius;
        float y = sin(angle) * radius;

        outer.push_back(vec2(x, y));
    }

    // Monta triângulos entre o centro e cada par de vértices
    for (int i = 0; i < points * 2; ++i)
    {
        vec2 v1 = outer[i];
        vec2 v2 = outer[(i + 1) % (points * 2)];

        vertices.push_back(center);
        vertices.push_back(v1);
        vertices.push_back(v2);
    }
}

Shape2DStar::Shape2DStar(Transform2D &t, Color &c): Shape2DStar(t, c, 1.0f, 0.5f, 5)
{
    
}