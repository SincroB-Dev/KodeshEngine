#include "gameObject.h"
#include <utility>

GameObject::GameObject(const char *name, Transform2D *transform, Color color = {1.0f, 0.0f, 0.0f, 1.0f}):
    name(name), transform(*transform), color(color)
{
    shape = new Shape2DQuad(this->transform, this->color);
}
    
GameObject::GameObject(const char *name):
    name(name)
{}

GameObject::~GameObject()
{}

void GameObject::AttachShape(Shape2D *s)
{
    if (shape != nullptr)
    {
        delete shape;
    }

    shape = s;
}

void GameObject::applyMovement(Point mov)
{
    transform.localPosition += mov;
}
void GameObject::setLocalPosition(Point position)
{
    transform.localPosition = position;
}
void GameObject::setLocalScale(Point scale)
{
    transform.localScale = scale;
}
void GameObject::setLocalAngle(float angle)
{
    transform.localAngle = angle;
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