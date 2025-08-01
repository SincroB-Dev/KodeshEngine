#pragma once

#include "scene.h"

#include <map>
#include <string>
#include <iostream>
#include <SDL2/SDL.h>

#include "editor/gizmo2d.h"

using namespace scene::editor;

class SceneManager {
private:
    Scene *activeScene;
    Camera2D *camera;
    Gizmo2D *gizmos;

    int &mouseX, &mouseY;
    int &width, &height;
    
    std::map<std::string, Scene*> sceneList;

public:
    SceneManager(int &width, int &height, int &mouseX, int &mouseY);
    ~SceneManager();
    
    SDL_Event event;

    void Update();
    void Render();
    
    GameObject* GetMainObject() const { return nullptr; }
    Camera2D* GetActiveCamera() const { return camera; }
};
