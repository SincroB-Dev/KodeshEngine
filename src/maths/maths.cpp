#include "maths.h"

namespace maths
{
    vec2 normalized(vec2 vec)
    {
        float l = vec.length();
        if (l == 0.0f)
        {
            return vec2();
        }

        vec2 res = vec;

        res.x = vec.x != 0.0f ? vec.x / l : 0.0f;
        res.y = vec.y != 0.0f ? vec.y / l : 0.0f;

        return res;
    }    
    
    vec3 normalized(vec3 vec)
    {
        float l = vec.length();
        if (l == 0.0f)
        {
            return vec3();
        }

        vec3 res = vec;

        res.x = vec.x != 0.0f ? vec.x / l : 0.0f;
        res.y = vec.y != 0.0f ? vec.y / l : 0.0f;
        res.z = vec.z != 0.0f ? vec.z / l : 0.0f;

        return res;
    }    

    vec4 normalized(vec4 vec)
    {
        float l = vec.length();
        if (l == 0.0f)
        {
            return vec4();
        }

        vec4 res = vec;

        res.x = vec.x != 0.0f ? vec.x / l : 0.0f;
        res.y = vec.y != 0.0f ? vec.y / l : 0.0f;
        res.z = vec.z != 0.0f ? vec.z / l : 0.0f;
        res.w = vec.w != 0.0f ? vec.w / l : 0.0f;

        return res;
    } 
    
    vec2 rotate(const vec2& v, float angleRad)
    {
        float cosA = std::cos(angleRad);
        float sinA = std::sin(angleRad);

        return vec2{
            v.x * cosA - v.y * sinA,
            v.x * sinA + v.y * cosA
        };
    }
}