#pragma once

#include "color.h"
#include "point.h"
#include "transform2d.h"

#include "vec2.h"
#include "vec3.h"
#include "vec4.h"

#include "mat4.h"

namespace maths
{
    vec2 normalized(vec2 vec);
    vec3 normalized(vec3 vec);
    vec4 normalized(vec4 vec);

    vec2 rotate(const vec2& v, float angleRad);
}