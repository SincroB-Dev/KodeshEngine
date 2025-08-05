#include "gameObject.h"
#include <utility>

GameObject::GameObject(const char *name, Transform2D *transform, Color color = {1.0f, 0.0f, 0.0f, 1.0f}):
    Entity(name, *transform), color(color)
{
    shape = new Shape2DQuad(this->transform, this->color);
}
    
GameObject::GameObject(const char *name):
    Entity(name, Transform2D())
{}

GameObject::~GameObject()
{
    
}

void GameObject::AttachShape(Shape2D *s)
{
    if (shape != nullptr)
    {
        delete shape;
    }

    shape = s;
}

void GameObject::applyMovement(vec2 mov)
{
    transform.localPosition += mov;
}

void GameObject::update()
{
    // syncTransform(&transform);
}

void GameObject::render()
{
    if (shape)
    {
        shape->render();
    }
}