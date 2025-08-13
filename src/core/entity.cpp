#include "entity.h"

namespace core
{
    std::atomic<EntityID> Entity::entityCounter{1};
    
    Entity::Entity(const char* name, Transform2D transform): name(name), transform(transform),
    id(entityCounter++) 
    {}
    
    Entity::~Entity() 
    {}

    void Entity::setLocalPosition(vec2 position)
    {
        transform.localPosition = position;
    }
    void Entity::setLocalScale(vec2 scale)
    {
        transform.localScale = scale;
    }
    void Entity::setLocalAngle(float angle)
    {
        transform.localAngle = angle;
    }
    
    void Entity::setName(const char* name)
    {
        this->name = name;
    }
}
