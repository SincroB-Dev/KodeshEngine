#include "scene.h"

#include "../models/shapes.h"
#include "../ui/forms/logger.h"
#include "sceneManager.h"

#include <algorithm>
#include <iostream>
#include <sstream>
#include <iomanip>

std::atomic<SceneID> Scene::sceneCounter{1};

Scene::Scene(int& screenWidth, int& screenHeight)
    : screenWidth(screenWidth), screenHeight(screenHeight), id(sceneCounter++)
{
    if (mainCamera == nullptr)
    {
        mainCamera = new Camera2D("chd_kn_view", screenWidth, screenHeight);
    }
}

// Delega para o primeiro construtor
Scene::Scene(SceneManager& manager): Scene(manager.width, manager.height)
{}

Scene::~Scene() 
{
    delete mainCamera;
    mainCamera = nullptr;
    ClearScene();
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
        return;
    }
    
    std::string entityN = object->name;
    EntityID id = Entity::GetEntityCounter();
    std::ostringstream oss;
    oss << '.' << std::setw(3) << std::setfill('0') << id;
    
    object->setName((entityN + oss.str()).c_str());
    
    AddObject(object);
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

Scene* Scene::Clone()
{
    Scene* copy = new Scene(screenWidth, screenHeight);
    
    copy->activeObject = nullptr;
    copy->mainCamera = mainCamera->Clone();
    copy->id = id;
    
    for (auto it=objectList.begin(); it!=objectList.end(); it++)
    {
        copy->objectList.push_back((*it)->Clone());
    }
    
    return copy;
}

void Scene::Render()
{
    for (Entity* obj : objectList)
    {
        obj->update();
        obj->render();
    }
}
