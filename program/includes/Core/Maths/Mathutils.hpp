#pragma once

#include "Core/Maths/Vector.hpp"
#include "Core/Maths/Vector3.hpp"
#include "Core/Maths/Vector4.hpp"

namespace core
{
    namespace mathutils
    {
        Vector Normalized(Vector vec);
        Vector3 Normalized(Vector3 vec);
        Vector4 Normalized(Vector4 vec);

        Vector Rotate(const Vector& v, float angleRad);
    }
}