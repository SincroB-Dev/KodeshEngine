#include "Core/Maths/Color.hpp"

using json = nlohmann::json;

namespace core
{
    namespace utils
    {
        // [SERIALIZATION]
        json Color::serialize() const
        {
            json jcolor = {r, g, b, a};
            
            return jcolor;
        }

        // [DESERIALIZATION]
        Color Color::deserialize(const json& jcolor)
        {
            r = jcolor[0];
            g = jcolor[1];
            b = jcolor[2];
            a = jcolor[3];
            
            return *this;
        }
    }
}