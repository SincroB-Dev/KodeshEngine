#pragma once

#include <SDL2/SDL.h>
#include <GL/gl.h>

#include <iostream>
#include <string>

#include "../core/core.h"
#include "../ui/manager.h"

#include "engineController.h"

using namespace core::ui;

namespace core
{
    namespace controller
    {
        class KodeshController
        {
            SDL_Window* window;
            SDL_GLContext context;
            SDL_Event event;
            
            LuaInterpreter* lua_;
            
            // Gerenciadores
            SceneManager* sc_manager;
            UIManager* ui_manager;
            
            // Backup da cena, onde será executado o modo de jogo.
            SceneManager* sc_manager_bkp;
            
            // Controllers
            EngineController* engine;
            
            bool kodeshIsRunning;
            bool kodeshDisplayUI;
            
            int width, height;
            int mouseX, mouseY;
            
            friend class EngineController;
            
        public:
            KodeshController(const char* title, int w, int h, int si=1);
            ~KodeshController();
            
            void InitManagers();
            void InitControllers();
            
            void MainLoop();
            void EventLoop();
            
            void Update();
            void Render();
            void Destroy();
            
            inline void HideUI() { kodeshDisplayUI = false; };
            inline void ShowUI() { kodeshDisplayUI = true; };
        };
    }
}
