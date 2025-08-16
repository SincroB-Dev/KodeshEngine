#pragma once

#include "../maths/maths.h"

using namespace maths;

class Camera2D {
private:
    int _latest_swid;
    int _latest_shei;
    int _latest_space;

public:
    const char *name;

    int &screenWidth;
    int &screenHeight;

    float space = 10.0f;

    inline Camera2D(const char *name, int &swidth, int &sheight): 
        name(name), screenWidth(swidth), screenHeight(sheight) {}
    ~Camera2D() {}

    mat4 projection;
    mat4 view;

    vec2 worldMouse = {0.0f, 0.0f};
    vec4 position = {0.0f, 0.0f, 0.0f, 1.0f};

    void UpdateProjection();
    void UpdateView();
    void Update();
    
    Camera2D* Clone();

    mat4 GetViewProjection() const;
    void ApplyScreenToWorld(int mouseX, int mouseY);
    vec2 GetWorldMouse() const;
};
