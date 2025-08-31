#include "Core/Maths/Mathutils.hpp"

#include <math.h>

namespace core
{
    namespace mathutils
    {
        Vector Normalized(Vector vec)
        {
            float l = vec.length();
            if (l == 0.0f)
            {
                return Vector();
            }

            Vector res = vec;

            res.x = vec.x != 0.0f ? vec.x / l : 0.0f;
            res.y = vec.y != 0.0f ? vec.y / l : 0.0f;

            return res;
        }    
        
        Vector3 Normalized(Vector3 vec)
        {
            float l = vec.length();
            if (l == 0.0f)
            {
                return Vector3();
            }

            Vector3 res = vec;

            res.x = vec.x != 0.0f ? vec.x / l : 0.0f;
            res.y = vec.y != 0.0f ? vec.y / l : 0.0f;
            res.z = vec.z != 0.0f ? vec.z / l : 0.0f;

            return res;
        }    

        Vector4 Normalized(Vector4 vec)
        {
            float l = vec.length();
            if (l == 0.0f)
            {
                return Vector4();
            }

            Vector4 res = vec;

            res.x = vec.x != 0.0f ? vec.x / l : 0.0f;
            res.y = vec.y != 0.0f ? vec.y / l : 0.0f;
            res.z = vec.z != 0.0f ? vec.z / l : 0.0f;
            res.w = vec.w != 0.0f ? vec.w / l : 0.0f;

            return res;
        } 
        
        Vector Rotate(const Vector& v, float angleRad)
        {
            float cosA = std::cos(angleRad);
            float sinA = std::sin(angleRad);

            return Vector{
                v.x * cosA - v.y * sinA,
                v.x * sinA + v.y * cosA
            };
        }
    }
}