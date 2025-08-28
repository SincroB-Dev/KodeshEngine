#define SDL_MAIN_HANDLED
#define IMGUI_DEFINE_MATH_OPERATORS

#include "Core/Application/KodeshApplication.hpp"

#include "Core/Systems/SceneManager.hpp"
#include "Core/Systems/UILayerManager.hpp"

#include "Core/Helpers/LogManager.hpp"
#include "Platform/OpenGL/RendererFF.hpp"

#include <memory>

// Testes A/B
#include "Sandbox.hpp"

using namespace core::app;
using namespace core::systems;
using namespace core::renderer;
using namespace core::utils;

using namespace platform;

int main(int argc, char *argv[])
{
    KodeshApplication app;

    // Logger não deve ser registrado como um subsystem, pois ele
    // é apenas um storage system.
    LogManager::Main = new LogManager();

    // Renderizador Fixed-Functions OpenGL
    std::unique_ptr<RendererFF> renderer = std::make_unique<RendererFF>();

    app.UseRenderer(std::move(renderer));

    // Registra sistemas (UInterface, SceneManager, AudioManager, Physics...)
    app.RegisterSystem<SceneManager>();
    app.RegisterSystem<UILayerManager>("configs.json"/*path de configurações visuais*/);

    // Alguns subsistemas deve ser atualizados, como no caso de UILayerManager que utiliza
    // do SDL/GLFW para sua inicialização. (Atualmente apenas SDL foi implementado)
    UILayerManager* ui = app.GetSystem<UILayerManager>();
    
    ui->InitImGui((SDL_Window*)app.GetNativeWindow(), (SDL_GLContext*)app.GetContext());

    // Construção dos menus superiores com o UILayerManager
    sandbox::UserInterfaceModulation(app, *ui);
    sandbox::UserInterfaceWindows(app, *ui);

    // Criação da primeira cena da engine
    sandbox::InitialScene(app, *renderer.get());

    // Registra callbacks
    app.RegisterWindowCallbacks();

    app.Run();

    // Apaga o logmanager
    delete LogManager::Main;

    return 0;
}
