#pragma once

#include "../core/core.h"

#include "../../libs/imgui/imgui.h"
#include "../../libs/ImGuiFileDialog/ImGuiFileDialog.h"

class UIManager {
public:
    UIManager(SDL_Window *window, SDL_GLContext context);

    void InitImGui(SDL_Window *window, SDL_GLContext context);
    void LoadFonts(ImGuiIO &io);
    void SetTheme(ImGuiStyle& style, ImVec4* colors);
    void ProcessEvent(const SDL_Event *event);

    void InitShapeFactory();

    void ShapeSelector(SceneManager& scene);

    void Render(SceneManager& scene);
};