#pragma once

#include <nlohmann/json.hpp>
#include <memory>

#include "Core/Utils/Vector.hpp"


namespace core
{
    namespace utils
    {
        struct Transform2D
        {
            mathutils::Vector Position = {0.0f, 0.0f};
            mathutils::Vector Scale = {1.0f, 1.0f};
            float Angle = 0.0f, Z = 0.0f;

            nlohmann::json serialize() const;
            Transform2D deserialize(const nlohmann::json jtransform);
        };
    }
}