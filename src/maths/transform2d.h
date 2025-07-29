#ifndef TRANSFORM2D_H
#define TRANSFORM2D_H

#include "point.h"

struct Transform2D
{
    Point localPosition = {0.0f, 0.0f};
    Point localScale = {1.0f, 1.0f};
    float localAngle = 0.0f;

    void apply();
};

#endif