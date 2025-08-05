#ifndef ENTITY_H
#define ENTITY_H

#include "../maths/maths.h"

namespace core
{
    class Entity
    {
    public:
        Entity(const char *name, Transform2D transform);
        virtual ~Entity();
        
        const char *name;
        Transform2D transform;

        virtual void update() {};
        virtual void render() {};

        virtual void setLocalPosition(vec2 position) final;
        virtual void setLocalScale(vec2 scale) final;
        virtual void setLocalAngle(float angle) final;
    };
}

#endif
