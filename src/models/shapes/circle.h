#ifndef SHAPES_CIRCLE_H
#define SHAPES_CIRCLE_H

#include "shape.h"

class Shape2DCircle: public Shape2D
{
public:
    Shape2DCircle(Transform2D &t, Color &c);
    Shape2DCircle(Transform2D &t, Color &c, int vertices);

    inline virtual void Register() override
    {
        //REGISTER_FIELD(vertices, "Vertices");
    }
};

#endif