#include "gizmo2d.h"
#include "../sceneManager.h"

#include <GL/gl.h>

using namespace maths;

namespace core
{
    namespace editor
    {
        Gizmo2D::Gizmo2D(SceneManager* sceneManager):
              sceneManager(sceneManager), currentMode(Mode::None), isDragging(false) {}

        void Gizmo2D::DrawCircle(Color color, float radius) {
            color.apply();
            glBegin(GL_LINE_LOOP);
            for (int i = 0; i < 64; ++i) {
                float theta = 2.0f * M_PI * float(i) / 64.0f;
                float x = radius * cosf(theta);
                float y = radius * sinf(theta);
                glVertex2f(x, y);
            }
            glEnd();
        }

        void Gizmo2D::DrawSquare(vec2 pos, Color color) {
            float half = 0.1f;
            glColor3f(color.r, color.g, color.b);
            glBegin(GL_QUADS);
            glVertex2f(pos.x - half, pos.y - half);
            glVertex2f(pos.x + half, pos.y - half);
            glVertex2f(pos.x + half, pos.y + half);
            glVertex2f(pos.x - half, pos.y + half);
            glEnd();
        }

        void Gizmo2D::DrawSector(float fromAngle, float toAngle, float radius, Color color, int segments = 64) {
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

            // Corrigir o ângulo para loop contínuo (abrindo em pizza)
            float fullCircle = 2.0f * M_PI;
            float angleSpan = fmodf(toAngle - fromAngle, fullCircle);
            if (angleSpan < 0) angleSpan += fullCircle; // Garantir valor positivo

            radius *= 0.9f;

            color.apply();
            glBegin(GL_TRIANGLE_FAN);
            glVertex2f(0.0f, 0.0f); // centro

            float step = angleSpan / segments;
            for (int i = 0; i <= segments; ++i) {
                float theta = fromAngle + step * i;
                glVertex2f(cosf(theta) * radius, sinf(theta) * radius);
            }
            glEnd();

            glDisable(GL_BLEND);
        }

        void Gizmo2D::DrawArrow(vec2 dir, Color color) {
            color.apply();
            glBegin(GL_LINES);
            glVertex2f(0, 0);
            glVertex2f(dir.x, dir.y);
            glEnd();

            // Cabeça da seta
            vec2 ortho(-dir.y, dir.x); // perpendicular
            vec2 headBase = dir * 0.8f;

            glBegin(GL_TRIANGLES);
            glVertex2f(dir.x, dir.y);
            glVertex2f(headBase.x + ortho.x * 0.125f, headBase.y + ortho.y * 0.125f);
            glVertex2f(headBase.x - ortho.x * 0.125f, headBase.y - ortho.y * 0.125f);
            glEnd();
        }        

        bool Gizmo2D::HitTestCircle(vec2 mousePos, float radius) {
            return (mousePos - transform->localPosition).length() < (radius + 0.1f) &&
                (mousePos - transform->localPosition).length() > (radius - 0.2f);
        }

        bool Gizmo2D::HitTestSquare(vec2 mousePos, vec2 localPos) {
            vec2 diff = mousePos - transform->localPosition;
            return abs(diff.x) < 0.1f && abs(diff.y) < 0.1f;
        }
        
        bool Gizmo2D::HitTestArrow(vec2 dir, vec2 mousePos, float threshold) {
            vec2 local = mousePos - transform->localPosition;
            float proj = local.dot(dir);
            vec2 closest = dir * proj;
            float distance = (local - closest).length();
            return (proj > 0.2f && proj < 1.2f && distance < threshold);
        }

        void Gizmo2D::Update() {
            if (transform != nullptr)
            {
                vec2 worldMouse = sceneManager->GetActiveCamera()->GetWorldMouse();
                SDL_Event &event = sceneManager->event;
    
                if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
                    isDragging = false;
    
                    if (HitTestArrow(vec2(1, 0), worldMouse)) {
                        currentMode = Mode::TranslateX;
                    } else if (HitTestArrow(vec2(0, 1), worldMouse)) {
                        currentMode = Mode::TranslateY;
                    } else if (HitTestCircle(worldMouse, 1.2f)) {
                        currentMode = Mode::Rotate;
                    } else if (HitTestSquare(worldMouse, vec2(1.5f, 0))) {
                        currentMode = Mode::ScaleX;
                    } else if (HitTestSquare(worldMouse, vec2(0, 1.5f))) {
                        currentMode = Mode::ScaleY;
                    } else {
                        currentMode = Mode::None;
                    }
    
                    if (currentMode != Mode::None) {
                        isDragging = true;
                        dragStartMouse = worldMouse;
    
                        if (currentMode == Mode::Rotate)
                        {
                            dragStartValue = vec2(transform->localAngle, 0);
                            isRotating = true;
                        }
                        else if (currentMode == Mode::ScaleX || currentMode == Mode::ScaleY)
                            dragStartValue = transform->localScale;
                        else
                            dragStartValue = transform->localPosition;
                    }
    
                } else if (event.type == SDL_MOUSEBUTTONUP && event.button.button == SDL_BUTTON_LEFT) {
                    isDragging = false;
                    isRotating = false;
                    currentMode = Mode::None;
                }
    
                if (event.type == SDL_MOUSEMOTION && isDragging) {
                    vec2 delta = worldMouse - dragStartMouse;
    
                    switch (currentMode) {
                        case Mode::TranslateX:
                            transform->localPosition.x = dragStartValue.x + delta.x;
                            break;
                        case Mode::TranslateY:
                            transform->localPosition.y = dragStartValue.y + delta.y;
                            break;
                        case Mode::Rotate:
                            transform->localAngle = dragStartValue.x + ((delta.x + delta.y) * (180.0f / M_PI));
                            break;
                        case Mode::ScaleX:
                            transform->localScale.x = std::max(0.1f, dragStartValue.x + delta.x);
                            break;
                        case Mode::ScaleY:
                            transform->localScale.y = std::max(0.1f, dragStartValue.y + delta.y);
                            break;
                        default:
                            break;
                    }
                }
            }
        }

        void Gizmo2D::Render() {
            if (transform != nullptr)
            {
                glPushMatrix();
                glTranslatef(transform->localPosition.x, transform->localPosition.y, 0);

                if (isRotating) {
                    DrawSector(0.0f, transform->localAngle * (M_PI / 180.0f), 1.2f, Color(1.0f, 0.7f, 0.3f, 0.5f)); // cor laranja translúcida
                }

                // Eixos de translação (fixos)
                DrawArrow(vec2(1, 0), currentMode == Mode::TranslateX ? Color(1, 0.5f, 0.5f) : Color(1, 0, 0)); // X
                DrawArrow(vec2(0, 1), currentMode == Mode::TranslateY ? Color(0.5f, 1, 0.5f) : Color(0, 1, 0)); // Y

                // Linha circular de rotação (laranja)
                Color rotColor = currentMode == Mode::Rotate ? Color(1.0f, 0.7f, 0.3f) : Color(1.0f, 0.5f, 0.0f);
                DrawCircle(rotColor, 1.2f);

                // Quadradinhos para escala
                DrawSquare(vec2(1.5f, 0), currentMode == Mode::ScaleX ? Color(1, 0.6f, 1) : Color(1, 0, 1));
                DrawSquare(vec2(0, 1.5f), currentMode == Mode::ScaleY ? Color(0.6f, 1, 1) : Color(0, 1, 1));

                glPopMatrix();
            }
        }
    
        void Gizmo2D::SetTransform(Transform2D *transform)
        {
            this->transform = transform;
        }
    };
};
