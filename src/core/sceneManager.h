#pragma once

#include "scene.h"

#include <map>
#include <string>
#include <iostream>
#include <SDL2/SDL.h>

#include "editor/gizmo2d.h"
#include "../maths/maths.h"
#include "components/componentStorage.h"

using namespace scene::editor;
using namespace maths;
using namespace core::components;

class SceneManager {
private:
    Scene *activeScene;
    Camera2D *camera;
    Gizmo2D *gizmos;
    ComponentStorage components;

    int &width, &height;
    int &mouseX, &mouseY;
    
    std::map<std::string, Scene*> sceneList;

    vec2 dragStartMouse;
    vec2 *worldMouse;

    bool dragCam = false;

    void CameraControls();

public:
    SceneManager(int &width, int &height, int &mouseX, int &mouseY);
    ~SceneManager();
    
    SDL_Event event;

    void UpdateWhenEvent();
    void Update();
    void Render();
    
    GameObject* GetMainObject() const { return nullptr; }
    Camera2D* GetActiveCamera() const { return camera; }
    Scene* GetActiveScene() const { return activeScene; }
};
