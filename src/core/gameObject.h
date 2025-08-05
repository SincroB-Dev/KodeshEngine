#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "../models/shapes.h"
#include "entity.h"
#include <memory>

using namespace core;

class GameObject : public Entity
{
public:
    GameObject(const char *name);
    GameObject(const char *name, Transform2D *transform, Color color);
    virtual ~GameObject();

    Color color;
    Shape2D *shape = nullptr;

    virtual void update() override;
    virtual void render() override;

    virtual void applyMovement(vec2 mov) final;
    virtual void AttachShape(Shape2D *shape) final;
};

#endif