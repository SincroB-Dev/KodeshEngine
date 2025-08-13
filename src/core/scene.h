#ifndef SCENE_H
#define SCENE_H

#include "gameObject.h"
#include <vector>

#include "editor/grid.h"
#include "camera2d.h"

using namespace core;

class Scene final
{
    int &screenWidth, &screenHeight;
    
public:
    const char* name;
    Camera2D* mainCamera = nullptr;
    Entity* activeObject = nullptr;

    inline Scene(int &screenWidth, int &screenHeight)
        : screenWidth(screenWidth), screenHeight(screenHeight), name("scene")
    {
        if (mainCamera == nullptr)
        {
            mainCamera = new Camera2D("chd_kn_view", screenWidth, screenHeight);
        }
    }
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

private:
    std::vector<Entity*> objectList;

    scene::editor::GridView gridView;
};

#endif
