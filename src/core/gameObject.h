#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "../models/shapes.h"
#include <memory>

class GameObject
{
public:
    GameObject(const char *name);
    GameObject(const char *name, Transform2D *transform, Color color);
    virtual ~GameObject();

    const char *name;

    Transform2D transform;
    Color color;
    Shape2D *shape = nullptr;

    virtual void update();

    virtual void render();

    virtual void applyMovement(vec2 mov) final;
    
    virtual void setLocalPosition(vec2 position) final;
    virtual void setLocalScale(vec2 scale) final;
    virtual void setLocalAngle(float angle) final;

    virtual void AttachShape(Shape2D *shape) final;
};

#endif