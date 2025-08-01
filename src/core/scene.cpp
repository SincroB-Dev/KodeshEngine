#include "scene.h"
#include "../models/shapes.h"

#include <algorithm>
#include <iostream>

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

Camera2D *Scene::AddCamera(const char *name)
{
    if (GetCamera(name) == nullptr)
    {
        Camera2D *camera = new Camera2D(name, screenWidth, screenHeight);
    
        if (mainCamera == nullptr)
        {
            mainCamera = camera;
        }
    
        cameraList.push_back(camera);
        return camera;

    } return nullptr;
}

Camera2D *Scene::GetCamera(const char *name)
{
    auto it = std::find_if(cameraList.begin(), cameraList.end(), [name](Camera2D *obj){
        return obj->name == name;
    });

    if (it != cameraList.end())
    {
        return (*it);
    }
    else
        return nullptr;
}

void Scene::RemoveCamera(const char *name)
{
}

void Scene::Render()
{
    gridView.Draw();

    for (GameObject *obj : objectList)
    {
        obj->update();
        obj->render();
    }
}