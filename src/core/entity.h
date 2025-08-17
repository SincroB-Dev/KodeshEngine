#ifndef ENTITY_H
#define ENTITY_H

#include "../maths/maths.h"
#include <atomic>
#include <string>

namespace core
{
    using EntityID = uint32_t;

    class Entity
    {
        static std::atomic<EntityID> entityCounter;
        
    public:
        Entity(const char *name, Transform2D transform);
        virtual ~Entity();

        // Principais propriedades
        std::string name;
        Transform2D transform;

        virtual void update() {};
        virtual void render() {};

        virtual void setLocalPosition(vec2 position) final;
        virtual void setLocalScale(vec2 scale) final;
        virtual void setLocalAngle(float angle) final;

        inline virtual EntityID GetID() const final { return id; }
        inline virtual const char* getName() const final { return name.c_str(); };
        virtual void setName(const char* name) final;
        
        virtual Entity* Clone();
        
        inline static EntityID GetEntityCounter() { return entityCounter.load(); }
        
    private:
        const EntityID id;
    };
}

#endif
