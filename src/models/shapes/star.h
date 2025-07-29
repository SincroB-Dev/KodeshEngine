#ifndef SHAPE2DSTAR_H
#define SHAPE2DSTAR_H

#include "shape.h"

class Shape2DStar: public Shape2D
{
public:
    Shape2DStar(Transform2D &t, Color &c);
    Shape2DStar(Transform2D &t, Color &c, float Re, float Ri, int points);

    inline virtual void Register() override
    {}
};

#endif