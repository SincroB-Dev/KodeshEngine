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
    SceneManager(int& width, int& height, int& mouseX, int& mouseY);
    ~SceneManager(); 
    
    SDL_Event event;

    void UpdateWhenEvent();
    void Update();
    void Render();
    
    
    const char* GetActiveSceneName() const;
};
