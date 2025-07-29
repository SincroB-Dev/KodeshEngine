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

using ShapeFactory = std::function<std::unique_ptr<Shape2D>(Transform2D &, Color &)>;

class UIManager {
    // IO -> Instance Objects
    std::map<std::string, ShapeFactory> UI_IOShapes;

    int UI_ShapeSelectedIndex = -1;
    std::vector<std::string> UI_ShapeNames;

public:
    UIManager();

    void InitShapeFactory();

    void ShapeSelector(SceneManager& scene);

    void Render(SceneManager& scene);
};