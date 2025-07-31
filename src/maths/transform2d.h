#ifndef TRANSFORM2D_H
#define TRANSFORM2D_H

#include "vec2.h"

using namespace maths;

struct Transform2D
{
    vec2 localPosition = {0.0f, 0.0f};
    vec2 localScale = {1.0f, 1.0f};
    float localAngle = 0.0f;

    void apply();
};

#endif