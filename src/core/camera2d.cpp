#include "camera2d.h"

void Camera2D::Resize(int width, int height) {
    screenWidth = width;
    screenHeight = height;
    projection = mat4::OrthoAspect(space, width, height);
}

void Camera2D::UpdateView() {
    view = mat4::Translation(-position.x, -position.y, -position.z);
}

mat4 Camera2D::GetViewProjection() const {
    return projection * view;
}