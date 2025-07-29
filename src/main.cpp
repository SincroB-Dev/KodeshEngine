#include <SDL2/SDL.h>
#include <GL/gl.h>

#include "core/core.h"
#include "ui/manager.h"

#include "../libs/imgui/imgui.h"
#include "../libs/imgui/backends/imgui_impl_sdl2.h"
#include "../libs/imgui/backends/imgui_impl_opengl2.h"
#include "../libs/ImGuiFileDialog/ImGuiFileDialog.h"

void AdjustScreen(int space, int width, int height)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    if (width > height)
        glOrtho(-space, space, -space * height / width, space * height / width, -space, space);
    else
        glOrtho(-space * width / height, space * width / height, -space, space, -space, space);

    glMatrixMode(GL_MODELVIEW);
}

int main()
{
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window* window = SDL_CreateWindow("SDL + OpenGL 1.4", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 700, 500, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(window);
    SDL_GL_SetSwapInterval(1);

    int width, height;
    SDL_GetWindowSize(window, &width, &height);
    AdjustScreen(10.0f, width, height);

    // IMGUI Init
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    io.Fonts->Clear();
    io.Fonts->AddFontFromFileTTF("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf", 12.0f);

    ImGui::StyleColorsDark();
    ImGuiStyle &style = ImGui::GetStyle();
    style.WindowRounding = 2.0f;
    style.FrameRounding = 2.0f;
    style.ScrollbarRounding = 2.0f;
    style.FramePadding = ImVec2(6, 4);
    style.ItemSpacing = ImVec2(10, 6);

    ImGui_ImplSDL2_InitForOpenGL(window, context);
    ImGui_ImplOpenGL2_Init();

    // Inicializar gerenciadores
    UIManager ui;
    SceneManager scene;

    bool running = true;
    SDL_Event event;

    while (running)
    {
        while (SDL_PollEvent(&event))
        {
            ImGui_ImplSDL2_ProcessEvent(&event);
            if (event.type == SDL_QUIT) running = false;
        }

        ImGui_ImplOpenGL2_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();

        ui.Render(scene);  // passa a cena para a UI controlar

        glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        scene.Render();

        ImGui::Render();
        ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
        SDL_GL_SwapWindow(window);
    }

    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
