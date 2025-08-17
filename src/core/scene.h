#ifndef SCENE_H
#define SCENE_H

#include "gameObject.h"
#include <vector>

#include "camera2d.h"
#include <atomic>

using namespace core;

using SceneID = uint32_t;

class SceneManager;

class Scene final
{
    static std::atomic<SceneID> sceneCounter;
    int &screenWidth, &screenHeight;
    
public:
    SceneID id;
    Camera2D* mainCamera = nullptr;
    Entity* activeObject = nullptr;

    Scene(int& screenWidth, int& screenHeight);
    Scene(SceneManager& manager);
    ~Scene();

    // Gerenciamento de Objetos
    void AddObject(Entity *object);
    Entity* AddGameObject(const char *name, Shape2D *shape = NULL);

    Entity* GetObject(const char *name);
    std::vector<Entity*>& GetObjectList();

    void RemoveObject(const char *name);
    void ClearScene();

    void Update();
    void Render();
    
    Scene* Clone();

private:
    std::vector<Entity*> objectList;
};

#endif
