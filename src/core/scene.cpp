#include "scene.h"
#include "../models/shapes.h"

#include <algorithm>
#include <iostream>

Scene::Scene()
{
    objectList = new std::vector<GameObject*>();
}

Scene::~Scene() {}

GameObject *Scene::GetObject(const char *name)
{
    auto it = std::find_if(objectList->begin(), objectList->end(), [name](GameObject *obj){
        return obj->name == name;
    });

    if (it != objectList->end())
    {
        return (*it);
    }
    else
        return nullptr;
}
    
void Scene::AddObject(GameObject *object)
{
    if (GetObject(object->name) == nullptr)
    {
        objectList->push_back(object);
    }
}

GameObject *Scene::AddObject(const char *name, Shape2D *shape)
{
    GameObject *it = new GameObject(name);
    it->AttachShape(shape);
    AddObject(it);
    return it;
}

void Scene::render()
{
    for (GameObject *obj : *objectList)
    {
        obj->update();
        obj->render();
    }
}