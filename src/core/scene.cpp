#include "scene.h"
#include "../models/shapes.h"

#include <algorithm>
#include <iostream>

Scene::~Scene() 
{
    delete mainCamera;
    mainCamera = nullptr;
}

Entity* Scene::GetObject(const char *name)
{
    auto it = std::find_if(objectList.begin(), objectList.end(), [name](Entity *obj){
        return obj->name == name;
    });

    if (it != objectList.end())
    {
        return (*it);
    }
    else
        return nullptr;
}
    
void Scene::AddObject(Entity *object)
{
    if (GetObject(object->name) == nullptr)
    {
        objectList.push_back(object);
    }
}

Entity *Scene::AddGameObject(const char *name, Shape2D *shape)
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

void Scene::Update()
{

}

void Scene::Render()
{
    gridView.Draw();

    for (Entity *obj : objectList)
    {
        obj->update();
        obj->render();
    }
}