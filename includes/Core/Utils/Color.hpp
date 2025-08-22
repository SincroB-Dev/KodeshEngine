#pragma once

#include <iostream>
#include <memory>

#include <nlohmann/json.hpp>

namespace core
{
    namespace utils
    {
        struct Color
        {
            float r=1.0f, g=1.0f, b=1.0f, a=1.0f;

            Color() = default;
            Color(float r, float g, float b, float a = 1.0f)
                : r(r), g(g), b(b), a(a) {}

            nlohmann::json serialize() const;
            Color deserialize(const nlohmann::json& jcolor);
            
            inline friend std::ostream& operator<<(std::ostream &os, const Color &c)
            {
                os << "<Color: R " << c.r << ", G " << c.g << ", B " << c.b << ", A" << c.a << ">";
                return os;
            }
        };
    }
}
