#ifndef SHAPES_QUAD_H
#define SHAPES_QUAD_H

#include "shape.h"

class Shape2DQuad: public Shape2D
{
public:
    Shape2DQuad(Transform2D &t, Color &c);

    inline virtual void Register() override
    {}
};

#endif