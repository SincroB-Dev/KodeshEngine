#include "kodeshController.h"

namespace core
{
    namespace controller
    {
        KodeshController::KodeshController(const char* title, int w, int h, int si)
            : kodeshIsRunning(true)
        {
            SDL_Init(SDL_INIT_VIDEO);

            window = SDL_CreateWindow(
                title, 
                SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
                w, h, SDL_WINDOW_OPENGL
            );
            
            context = SDL_GL_CreateContext(window);
            
            SDL_GL_SetSwapInterval(si);
            SDL_GetWindowSize(window, &width, &height);
        }
        
        KodeshController::~KodeshController()
        {
            delete sc_manager;
            delete ui_manager;
        }
        
        void KodeshController::InitManagers()
        {
            // Inicializar gerenciadores
            sc_manager = new SceneManager(width, height, mouseX, mouseY);
            ui_manager = new UIManager(window, context, *sc_manager);
            
            LogWindow::Log("Kodesh Managers inicializados com sucesso!", LogType::INFO);
        }
        
        void KodeshController::EventLoop()
        {
            while (SDL_PollEvent(&sc_manager->event))
            {
                SDL_GetMouseState(&mouseX, &mouseY);

                ui_manager->ProcessEvent(&sc_manager->event);
                sc_manager->UpdateWhenEvent();

                if (sc_manager->event.type == SDL_QUIT)
                { 
                    kodeshIsRunning = false;
                }
            }
        }
        
        void KodeshController::Render()
        {
            sc_manager->Render();
            ui_manager->Render(*sc_manager);
        }
        
        void KodeshController::Update()
        {
            sc_manager->Update();
        }
        
        void KodeshController::MainLoop()
        {
            while (kodeshIsRunning)
            {
                EventLoop();
                Update();

                glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
                glClear(GL_COLOR_BUFFER_BIT);

                Render();

                SDL_GL_SwapWindow(window);
            }
        }
        
        void KodeshController::Destroy()
        {
            SDL_GL_DeleteContext(context);
            SDL_DestroyWindow(window);
            SDL_Quit();
        }
    }
}
