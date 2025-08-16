#pragma once

#include <SDL2/SDL.h>

#include "../../maths/maths.h"

using namespace maths;

class Camera2D;

namespace core
{
    namespace editor
    {
        class Editor;
        class CameraHandler
        {
            float sensibility;
            Editor* editor;
            
            bool isDragging, ctrlActive;
            vec2 dragStartMouse;
            int scrollMouse;
            
        public:
            CameraHandler(Editor* editor, float sensibility);
            ~CameraHandler();
            
            void Movement(int mouseX, int mouseY);
            void Zoom();
            void Apply(Camera2D& camera, int mouseX, int mouseY);
        };
    }
}
