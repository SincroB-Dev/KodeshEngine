#include "camera2d.h"

void Camera2D::UpdateProjection()
{
    if (_latest_shei != screenHeight || _latest_swid != screenWidth || _latest_space != space)
    {
        _latest_shei = screenHeight;
        _latest_swid = screenWidth;
        _latest_space = space;

        projection = mat4::OrthoAspect(space, screenWidth, screenHeight);
    }
}

void Camera2D::UpdateView() 
{
    view = mat4::Translation(-position.x, -position.y, -position.z);
}

void Camera2D::Update()
{
    UpdateProjection();
    UpdateView();
}

mat4 Camera2D::GetViewProjection() const 
{
    return projection * view;
}

void Camera2D::ApplyScreenToWorld(int mouseX, int mouseY)
{
    // Coordenadas normalizadas da tela (0 a 1)
    float normX = (float)mouseX / (float)screenWidth;
    float normY = (float)mouseY / (float)screenHeight;

    // Coordenadas do mundo (com ajuste de espaço e proporção)
    float worldX, worldY;

    if (screenWidth >= screenHeight) {
        // Horizontal dominante
        float viewWidth = space * 2.0f;
        float viewHeight = viewWidth * (float)screenHeight / (float)screenWidth;

        worldX = (normX * viewWidth) - space;
        worldY = (1.0f - normY) * viewHeight - (viewHeight / 2.0f);
    } else {
        // Vertical dominante
        float viewHeight = space * 2.0f;
        float viewWidth = viewHeight * (float)screenWidth / (float)screenHeight;

        worldX = (normX * viewWidth) - (viewWidth / 2.0f);
        worldY = (1.0f - normY) * viewHeight - space;
    }

    // Adiciona a posição da câmera
    worldX += position.x;
    worldY += position.y;

    worldMouse = vec2(worldX, worldY);
}

vec2 Camera2D::GetWorldMouse() const
{
    return worldMouse;
}
