#pragma once

#include "../maths/maths.h"

using namespace maths;

class Camera2D {
public:
    float space = 10.0f;

    int &screenWidth;
    int &screenHeight;

    inline Camera2D(int &swidth, int &sheight): screenWidth(swidth), screenHeight(sheight) {}

    mat4 projection;
    mat4 view;

    vec4 position = {0.0f, 0.0f, 0.0f, 1.0f};

    void Resize(int width, int height);
    void UpdateView();
    mat4 GetViewProjection() const;
    vec2 ScreenToWorld(int mouseX, int mouseY) const;
};
