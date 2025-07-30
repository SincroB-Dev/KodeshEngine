#include "scene.h"
#include "../models/shapes.h"

#include <algorithm>
#include <iostream>

Scene::Scene()
{}

Scene::~Scene() {}

GameObject *Scene::GetObject(const char *name)
{
    auto it = std::find_if(objectList.begin(), objectList.end(), [name](GameObject *obj){
        return obj->name == name;
    });

    if (it != objectList.end())
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
        objectList.push_back(object);
    }
}

GameObject *Scene::AddObject(const char *name, Shape2D *shape)
{
    GameObject *it = new GameObject(name);
    it->AttachShape(shape);
    AddObject(it);
    return it;
}

void Scene::RemoveObject(const char *name)
{
    for (auto it = objectList.begin(); it != objectList.end();)
    {
        if ((*it)->name == name)
        {
            delete *it;
            it = objectList.erase(it);
            break;
        }
        it++;
    }
}

void Scene::ClearScene()
{
    for (auto it = objectList.begin(); it != objectList.end();)
    {
        delete *it;
        it++;
    }

    objectList.clear();
}

void Scene::render()
{
    gridView.Draw();

    for (GameObject *obj : objectList)
    {
        obj->update();
        obj->render();
    }
}