#pragma once

#include <SDL2/SDL.h>
#include <GL/gl.h>

#include <iostream>
#include <string>

#include "../core/core.h"
#include "../ui/manager.h"

using namespace core::ui;

namespace core
{
    namespace controller
    {
        class KodeshController
        {
            SDL_Window* window;
            SDL_GLContext context;
            
            // Gerenciadores
            SceneManager* sc_manager;
            UIManager* ui_manager;
            
            bool kodeshIsRunning;
            
            int width, height;
            int mouseX, mouseY;
            
        public:
            KodeshController(const char* title, int w, int h, int si=1);
            ~KodeshController();
            
            void InitManagers();
            void MainLoop();
            void EventLoop();
            void Render();
            void Update();
            void Destroy();
        };
    }
}
