#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <GL/gl.h>

#include <iostream>
#include "core/core.h"
#include "ui/manager.h"

using namespace core::ui;

int main(int argc, char *argv[])
{
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window* window = SDL_CreateWindow("SDL + OpenGL 1.4", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 950, 500, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(window);
    SDL_GL_SetSwapInterval(1);

    int width, height;
    int mouseX, mouseY;

    bool running = true;

    SDL_GetWindowSize(window, &width, &height);

    // Inicializar gerenciadores
    UIManager ui(window, context);
    SceneManager sm(width, height, mouseX, mouseY);

    while (running)
    {
        while (SDL_PollEvent(&sm.event))
        {
            SDL_GetMouseState(&mouseX, &mouseY);

            ui.ProcessEvent(&sm.event);
            sm.UpdateWhenEvent();

            if (sm.event.type == SDL_QUIT)
            { 
                running = false;
            }
        }

        glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        sm.Update();

        sm.Render();
        ui.Render(sm);

        SDL_GL_SwapWindow(window);
    }

    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();
    
    return 0;
}
