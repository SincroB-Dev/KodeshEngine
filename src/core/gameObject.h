#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "../models/shapes.h"
#include <memory>

class GameObject
{
public:
    GameObject(const char *name);
    GameObject(const char *name, Transform2D *transform, Color color);
    ~GameObject();

    const char *name;

    Transform2D transform;
    Color color;
    Shape2D *shape = nullptr;

    virtual void update();

    virtual void render();

    virtual void applyMovement(Point mov) final;
    
    virtual void setLocalPosition(Point position) final;
    virtual void setLocalScale(Point scale) final;
    virtual void setLocalAngle(float angle) final;

    virtual void AttachShape(Shape2D *shape) final;
};

#endif