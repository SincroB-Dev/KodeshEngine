#pragma once

#include "scene.h"

#include <map>
#include <string>
#include <iostream>
#include <SDL2/SDL.h>

#include "editor/gizmo2d.h"
#include "../maths/maths.h"

using namespace scene::editor;
using namespace maths;

class SceneManager {
private:
    Scene *activeScene;
    Camera2D *camera;
    Gizmo2D *gizmos;

    int &width, &height;
    int &mouseX, &mouseY;
    
    std::map<std::string, Scene*> sceneList;

    vec2 dragStartMouse;
    vec2 *worldMouse;

    void CameraControls();

public:
    SceneManager(int &width, int &height, int &mouseX, int &mouseY);
    ~SceneManager();
    
    SDL_Event event;

    void Update();
    void Render();
    
    GameObject* GetMainObject() const { return nullptr; }
    Camera2D* GetActiveCamera() const { return camera; }
};
