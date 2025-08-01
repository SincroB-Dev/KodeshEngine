#pragma once

#include "../core/core.h"

#include "../../libs/imgui/imgui.h"
#include "../../libs/ImGuiFileDialog/ImGuiFileDialog.h"

#include "dockedToolbox.h"

using namespace core::ui;

class UIManager {
    DockedToolbox dockedToolbox;
    ImFontConfig icons_config;

    // Intervalo de caracteres da fonte de ícones (Material Icons)
    static const ImWchar icons_ranges[3]; // unicode private use area

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