#pragma once

#include <SDL2/SDL.h>

#include "../../maths/maths.h"

using namespace maths;

class Camera2D;

namespace core
{
    namespace editor
    {
        class CameraHandler
        {
            float sensibility;
            SDL_Event& event;
            
            bool isDragging, ctrlActive;
            vec2 dragStartMouse;
            int scrollMouse;
            
        public:
            CameraHandler(SDL_Event& event, float sensibility);
            ~CameraHandler();
            
            void Movement(int mouseX, int mouseY);
            void Zoom();
            void Apply(Camera2D& camera, int mouseX, int mouseY);
        };
    }
}
