#pragma once

#include "scene.h"

class SceneManager {
    Scene* scene;
    GameObject* mainObject;

public:
    SceneManager(int &width, int &height);
    ~SceneManager();

    void Update();
    void Render();
    
    GameObject* GetMainObject() const { return mainObject; }
    Camera2D* GetMainCamera() const { return scene->mainCamera; }
};
