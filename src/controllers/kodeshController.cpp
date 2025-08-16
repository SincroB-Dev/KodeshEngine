#include "kodeshController.h"

namespace core
{
    namespace controller
    {
        KodeshController::KodeshController(const char* title, int w, int h, int si)
            : kodeshIsRunning(true), kodeshDisplayUI(true)
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
            
            delete engine;
        }
        
        void KodeshController::InitManagers()
        {
            // Inicializar gerenciadores
            sc_manager = new SceneManager(width, height, mouseX, mouseY, event);
            ui_manager = new UIManager(window, context, *sc_manager);
            
            // Utilizado para execução in-game;
            sc_manager_bkp = nullptr;
            
            LogWindow::Log("Kodesh Managers inicializados com sucesso!", LogType::INFO);
        }
        
        void KodeshController::InitControllers()
        {
            // Inicializa controladores
            engine = new EngineController(*this);
        }
        
        void KodeshController::EventLoop()
        {
            while (SDL_PollEvent(&event))
            {
                SDL_GetMouseState(&mouseX, &mouseY);

                if (kodeshDisplayUI) ui_manager->ProcessEvent(&event);
                
                sc_manager->UpdateWhenEvent();

                if (event.type == SDL_QUIT)
                { 
                    kodeshIsRunning = false;
                }
                
                // Troca de estados da engine
                if (event.type == SDL_KEYDOWN)
                {
                    if (engine->state == EngineState::Edit)
                    {
                        if (event.key.keysym.sym == SDLK_F7)
                        {
                            engine->SwitchState(EngineState::Play);
                        }
                    }
                    else if (engine->state == EngineState::Play || engine->state == EngineState::Pause)
                    {
                        if (event.key.keysym.sym == SDLK_F8)
                        {
                            engine->SwitchState(EngineState::Edit);
                        }
                    }
                }
            }
        }
        
        void KodeshController::Render()
        {
            // Execução de cena backup, a cena que roda in-game
            if (engine->state == EngineState::Play || engine->state == EngineState::Pause)
            {
                if (sc_manager_bkp != nullptr)
                {
                    sc_manager_bkp->Render();
                }
            }
            // Execução da cena edit
            else
            {
                sc_manager->Render();
            }
            
            // Apenas é renderizado no modo de edição, ou no modo de pausa.
            if (kodeshDisplayUI)
            {
                ui_manager->Render(*sc_manager);
            }
            // Apresenta outra interface para modo de jogo.
            else
            {
            }
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
