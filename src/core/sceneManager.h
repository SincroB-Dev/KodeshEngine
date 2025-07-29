#pragma once

#include "scene.h"

class SceneManager {
    Scene* scene;
    GameObject* mainObject;

public:
    SceneManager();
    ~SceneManager();

    void Render();
    GameObject* GetMainObject() const { return mainObject; }
};
