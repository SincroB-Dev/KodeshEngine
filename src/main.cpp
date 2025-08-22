#define SDL_MAIN_HANDLED

#include "Core/Application/KodeshApplication.hpp"

#include "Core/Systems/SceneManager.hpp"
#include "Core/Systems/UILayer.hpp"
#include "Core/Scene/Scene.hpp"

#include "Core/ECS/Systems/RenderSystem.hpp"
#include "Core/ECS/TransformComponent.hpp"
#include "Core/ECS/ShapeComponent.hpp"
#include "Core/ECS/TagComponent.hpp"
#include "Core/ECS/InputComponent.hpp"
#include "Core/ECS/LifetimeComponent.hpp"

#include "Core/Events/EventDispatcher.hpp"
#include "Core/Events/KeyboardEvent.hpp"
#include "Core/Input/KeyState.hpp"

#include "Platform/OpenGL/RendererFF.hpp"

#include "Core/Renderer/RenderCommand.hpp"

#include "Core/Utils/Vector.hpp"
#include "Core/Utils/Color.hpp"

#include <SDL2/SDL.h>
#include <memory>

using namespace core::app;
using namespace core::events;
using namespace core::systems;
using namespace core::scene;
using namespace core::mathutils;
using namespace core::utils;
using namespace core::renderer;
using namespace core::input;
using namespace core;

using namespace platform;

void Sandbox(KodeshApplication& app, Renderer& renderer)
{
    // Demo (Sandbox)
    SceneManager* sm = app.GetSystem<SceneManager>();

    Scene* scn = sm->AddScene("Scene");
    ecs::EntityRegistry& entities = scn->GetRegistry();

    ecs::Entity object;
    for (int n=1; n<=10; n++)
    for (int i=1; i<=20; i++)
    {
        object = entities.CreateEntity();

        // Transformação de entities
        entities.AddComponent<ecs::TransformComponent>(
            object,
            Vector(-5.0f + i * 0.25f, -5.0f + n * 0.25f), Vector(0.2f, 0.2f), i*13.0f
        );

        // Componente gráfico dos entities
        entities.AddComponent<ecs::ShapeComponent>(
            object,
            (uint32_t)0, 
            Color(1.0f, 0.0f, 0.0f, 1.0f),
            PrimitiveType::Quads, 
            std::vector<utils::VertexInfo> {
                { Vector(-0.5f, -0.5f), Vector(0.0f, 0.0f) },
                { Vector( 0.5f, -0.5f), Vector(0.0f, 0.0f) },
                { Vector( 0.5f,  0.5f), Vector(0.0f, 0.0f) },
                { Vector(-0.5f,  0.5f), Vector(0.0f, 0.0f) },
            }, 
            0.0f
        );

        // Tempo de vida de entities
        entities.AddComponent<ecs::LifetimeComponent>(
            object,
            (((i % 2) + 1) * 5.0)
        );
    }

    object = entities.CreateEntity();

    ecs::TransformComponent& transform = entities.AddComponent<ecs::TransformComponent>(
        object,
        Vector(0.0f, -3.0f), Vector(1.0f, 1.0f), 0.0f
    );

    entities.AddComponent<ecs::ShapeComponent>(
        object,
        (uint32_t)0, 
        Color(0.0f, 1.0f, 0.0f, 1.0f),
        PrimitiveType::Quads, 
        std::vector<utils::VertexInfo> {
            { Vector(-0.5f, -0.5f), Vector(0.0f, 0.0f) },
            { Vector( 0.5f, -0.5f), Vector(0.0f, 0.0f) },
            { Vector( 0.5f,  0.5f), Vector(0.0f, 0.0f) },
            { Vector(-0.5f,  0.5f), Vector(0.0f, 0.0f) },
        }, 
        0.0f
    );

    ecs::InputComponent& input = entities.AddComponent<ecs::InputComponent>(
        object
    );

    input.AddAction("forward", SDLK_w, [&transform](double dt){
        transform.position.y += 10.0f * dt;
    }, KeyStateEnum::Held);

    input.AddAction("back", SDLK_s, [&transform](double dt){
        transform.position.y -= 10.0f * dt;
    }, KeyStateEnum::Held);

    input.AddAction("left", SDLK_a, [&transform](double dt){
        transform.position.x -= 10.0f * dt;
    }, KeyStateEnum::Held);

    input.AddAction("right", SDLK_d, [&transform](double dt){
        transform.position.x += 10.0f * dt;
    }, KeyStateEnum::Held);
}

int main(int argc, char *argv[])
{
    KodeshApplication app;

    // Renderizador Fixed-Functions OpenGL
    std::unique_ptr<RendererFF> renderer = std::make_unique<RendererFF>();

    app.UseRenderer(std::move(renderer));

    // Registra sistemas (UInterface, SceneManager, AudioManager, Physics...)
    app.RegisterSystem<UILayer>();
    app.RegisterSystem<SceneManager>();

    // Protocolo bicicleta de rodinha heuhuehuehue
    Sandbox(app, *renderer.get());

    // Registra callbacks
    app.RegisterWindowCallbacks();

    app.Run();
    
    return 0;
}
