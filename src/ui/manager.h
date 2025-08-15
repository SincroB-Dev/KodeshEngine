#pragma once

#include "../core/core.h"

#include "../../libs/imgui/imgui.h"
#include "../../libs/ImGuiFileDialog/ImGuiFileDialog.h"

#include "sidedock/dockedToolbox.h"

#include "forms/logger.h"
#include "forms/textEditor.h"

#include "../core/sceneManager.h"

namespace core
{
    namespace ui
    {
        class UIManager {
            SceneManager& sceneManager;
            DockedToolbox dockedToolbox;
            LuaTextEditor luaTextEditor;
        
            // Intervalo de caracteres da fonte de ícones (Material Icons)
            static const ImWchar icons_ranges[3]; // unicode private use area
            ImFontConfig icons_config;

        public:
            UIManager(SDL_Window *window, SDL_GLContext context, SceneManager& sm);
        
            void InitImGui(SDL_Window *window, SDL_GLContext context);
            
            void LoadFonts(ImGuiIO &io);
            void SetTheme(ImGuiStyle& style, ImVec4* colors);
            void ProcessEvent(const SDL_Event *event);
        
            void InitShapeFactory();
        
            void ShapeSelector(SceneManager& scene);
        
            void Render(SceneManager& scene);
        
            static ImFont* MainFont;
            static ImFont* IconifiedFont;
        };
    };
};
