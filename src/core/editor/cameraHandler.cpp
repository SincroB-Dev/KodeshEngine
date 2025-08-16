#include "cameraHandler.h"

#include "../camera2d.h"
#include "editor.h"
#include "../sceneManager.h"

namespace core
{
    namespace editor
    {
        CameraHandler::CameraHandler(Editor* editor, float sensibility)
            : editor(editor), sensibility(sensibility), isDragging(false), ctrlActive(false)
        {}
        
        CameraHandler::~CameraHandler()
        {}
        
        void CameraHandler::Movement(int mouseX, int mouseY)
        {
            SDL_Event& event = editor->sm.event;
            
            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_LCTRL)
            {
                ctrlActive = true;
            }
            if (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_LCTRL)
            {
                ctrlActive = false;
            }
            
            if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT)
            {
                if (ctrlActive && !isDragging)
                {
                    isDragging = true;
                    dragStartMouse = vec2(mouseX, mouseY);
                }
            }
            else if (event.type == SDL_MOUSEBUTTONUP && event.button.button == SDL_BUTTON_LEFT)
            {
                isDragging = false;
            }
        }
        
        void CameraHandler::Zoom()
        {
            SDL_Event& event = editor->sm.event;
            if (event.type == SDL_MOUSEWHEEL)
            {
                scrollMouse += event.wheel.y;
            }
        }
        
        void CameraHandler::Apply(Camera2D& camera, int mouseX, int mouseY)
        {
            if (isDragging)
            {
                vec2 delta = vec2(mouseX, mouseY) - dragStartMouse;

                camera.position.x -= delta.x * sensibility;
                camera.position.y += delta.y * sensibility;

                dragStartMouse = vec2(mouseX, mouseY);
            }
            
            if (scrollMouse != 0 && scrollMouse % 3 == 0)
            {
                camera.space += scrollMouse;
                
                scrollMouse = 0;
            }
        }
    }
}
