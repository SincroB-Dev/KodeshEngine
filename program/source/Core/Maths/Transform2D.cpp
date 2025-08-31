#include "Core/Maths/Transform2D.hpp"

using json = nlohmann::json;

namespace core
{
    namespace utils
    {
        // [SERIALIZATION]
        json Transform2D::serialize() const
        {
            json jtransform;
            
            jtransform["position"] = {Position.x, Position.y};
            jtransform["rotation"] = Angle;
            jtransform["scale"] = {Scale.x, Scale.y};
            
            return jtransform;
        }

        // [DESERIALIZATION]
        Transform2D Transform2D::deserialize(const json jtransform)
        {
            Position.x = jtransform["position"][0];
            Position.y = jtransform["position"][1];
            
            Angle = jtransform["rotation"];
            
            Scale.x = jtransform["scale"][0];
            Scale.y = jtransform["scale"][1];

            return *this;
        }
    }
}