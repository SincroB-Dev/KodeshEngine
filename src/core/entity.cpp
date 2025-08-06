#include "entity.h"

namespace core
{
    Entity::Entity(const char *name, Transform2D transform): name(name), transform(transform), id(134)
    {
    }
    
    Entity::~Entity()
    {
        delete name;
    }

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
}