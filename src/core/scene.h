#ifndef SCENE_H
#define SCENE_H

#include "gameObject.h"
#include <vector>

#include "editor/grid.h"
#include "camera2d.h"

class Scene final
{
    int &screenWidth, &screenHeight;

public:
    Camera2D *mainCamera;
    GameObject *activeObject;

    inline Scene(int &screenWidth, int &screenHeight)
        : screenWidth(screenWidth), screenHeight(screenHeight)
    {
        GameObject *mainObject = new GameObject("object");
        mainObject->AttachShape(new Shape2DStar(mainObject->transform, mainObject->color));
        activeObject = mainObject;

        AddObject(mainObject);
        AddCamera("camera");
    }
    ~Scene();

    // Gerenciamento de Objetos
    void AddObject(GameObject *object);
    GameObject *AddObject(const char *name, Shape2D *shape = NULL);
    
    GameObject *GetObject(const char *name);

    Camera2D *AddCamera(const char *name);
    Camera2D *GetCamera(const char *name);
    void RemoveCamera(const char *name);

    void RemoveObject(const char *name);
    void ClearScene();

    void Render();

private:
    std::vector<GameObject*> objectList;
    std::vector<Camera2D*> cameraList;

    scene::editor::GridView gridView;
};

#endif