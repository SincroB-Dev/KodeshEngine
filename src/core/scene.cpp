#include "scene.h"
#include "../models/shapes.h"

#include <algorithm>
#include <iostream>
#include "../ui/forms/logger.h"

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

std::vector<Entity*>& Scene::GetObjectList()
{
    return objectList;
}
    
void Scene::AddObject(Entity* object)
{
    if (GetObject(object->name.c_str()) == nullptr)
    {
        objectList.push_back(object);
        activeObject = object;
        
        core::ui::LogWindow::Log("Objeto adicionado a cena!", core::ui::LogType::INFO);
    }
}

Entity* Scene::AddGameObject(const char* name, Shape2D* shape)
{
    if (GetObject(name) == nullptr)
    {
        GameObject *it = new GameObject(name);
        it->AttachShape(shape);
        AddObject(it);
        return it;
    }
    return nullptr;
}

void Scene::RemoveObject(const char* name)
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
    for (Entity* obj : objectList)
    {
        obj->update();
        obj->render();
    }
}
