#pragma once

#include <string>
#include <memory>
#include <map>
#include <vector>
#include <utility>
#include <functional>

#include "../models/shapes.h"
#include "../maths/maths.h"
#include "../core/core.h"

#include "../../libs/imgui/imgui.h"
#include "../../libs/ImGuiFileDialog/ImGuiFileDialog.h"

using ShapeFactory = std::function<std::unique_ptr<Shape2D>(Transform2D &, Color &)>;

class UIManager {
    // IO -> Instance Objects
    std::map<std::string, ShapeFactory> UI_IOShapes;

    int UI_ShapeSelectedIndex = -1;
    std::vector<std::string> UI_ShapeNames;

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