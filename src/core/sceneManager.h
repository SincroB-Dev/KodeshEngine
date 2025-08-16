#pragma once

#include "scene.h"

#include <map>
#include <string>
#include <iostream>
#include <SDL2/SDL.h>

#include "editor/editor.h"
#include "../maths/maths.h"
#include "components/componentStorage.h"

using namespace maths;

using namespace core::editor;
using namespace core::components;

class SceneManager {
private:
    Scene *activeScene;
    Camera2D *camera;
    ComponentStorage components;
    Editor editor;

    int &width, &height;
    int &mouseX, &mouseY;
    
    std::map<std::string, Scene*> sceneList;

    
public:
    SceneManager(int& width, int& height, int& mouseX, int& mouseY, SDL_Event& event);
    SceneManager(SceneManager& clone);
    ~SceneManager(); 
    
    SDL_Event& event;
    bool EngineEditMode;

    void UpdateWhenEvent();
    void Update();
    void Render();
    
    SceneManager* Clone();
    
    inline GameObject* GetMainObject() const { return nullptr; }
    inline Camera2D* GetActiveCamera() const { return camera == nullptr ? activeScene->mainCamera : camera; }
    inline Scene* GetActiveScene() const { return activeScene; }
    
    const char* GetActiveSceneName() const;
};
