#pragma once

#include <SDL2/SDL.h>
#include "../../maths/maths.h"

class SceneManager;

using namespace maths;

namespace core
{
    namespace editor
    {
        class Gizmo2D {
        public:
            Gizmo2D(SceneManager *sceneManager);
        
            void Update();
            void Render();

            void SetTransform(Transform2D *transform);
        
        private:
            enum class Mode { None, TranslateX, TranslateY, Rotate, ScaleX, ScaleY };
            
            SceneManager *sceneManager = nullptr;
            Transform2D *transform = nullptr;

            Mode currentMode;
        
            vec2 position;
            float rotation;
            vec2 scale;
        
            mat4 viewProjection;
        
            bool isDragging;
            bool isRotating;
            vec2 dragStartMouse;
            vec2 dragStartValue;
        
            void DrawArrow(vec2 dir, Color color);
            void DrawCircle(Color color, float angle);
            void DrawSquare(vec2 size, Color color);
            void DrawSector(float fromAngle, float toAngle, float radius, Color color, int segments);
        
            bool HitTestArrow(vec2 dir, vec2 mousePos, float threshold = 0.2f);
            bool HitTestCircle(vec2 mousePos, float radius);
            bool HitTestSquare(vec2 mousePos, vec2 size);
        };
    };
};
