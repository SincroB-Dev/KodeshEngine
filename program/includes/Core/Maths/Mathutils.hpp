#pragma once

#include <glm/gtc/matrix_transform.hpp>

namespace core::mathutils
{
    inline glm::mat4 OrthoAspect(float size, float width, float height) 
    {
        float aspect = width / height;
        float half = size * 0.5f;
        return glm::ortho(
            -half * aspect, half * aspect,
            -half, half,
            -1.0f, 1.0f
        );
    }
}