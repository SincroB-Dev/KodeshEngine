#ifndef ENTITY_H
#define ENTITY_H

#include "../maths/maths.h"

namespace core
{
    using EntityID = uint32_t;

    class Entity
    {
        const EntityID id;

    public:
        Entity(const char *name, Transform2D transform);
        virtual ~Entity();

        // Principais propriedades
        const char *name;
        Transform2D transform;

        virtual void update() {};
        virtual void render() {};

        virtual void setLocalPosition(vec2 position) final;
        virtual void setLocalScale(vec2 scale) final;
        virtual void setLocalAngle(float angle) final;

        virtual EntityID GetID() const final { return id; }
    };
}

#endif
