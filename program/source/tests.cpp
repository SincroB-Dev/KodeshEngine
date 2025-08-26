#include "tests.hpp"

#include <math.h>

#include "Core/Application/KodeshApplication.hpp"

#include "Core/Systems/SceneManager.hpp"
#include "Core/Systems/UILayer.hpp"
#include "Core/Scene/Scene.hpp"

#include "Core/ECS/Systems/RenderSystem.hpp"
#include "Core/ECS/EntityRegistry.hpp"
#include "Core/ECS/TransformComponent.hpp"
#include "Core/ECS/ShapeComponent.hpp"
#include "Core/ECS/TagComponent.hpp"
#include "Core/ECS/InputComponent.hpp"
#include "Core/ECS/LifetimeComponent.hpp"

#include "Core/Events/EventDispatcher.hpp"
#include "Core/Events/KeyboardEvent.hpp"

#include "Core/Input/KeyState.hpp"

#include "Core/Systems/LogManager.hpp"
#include "Core/Renderer/RenderCommand.hpp"

#include "Core/Utils/Vector.hpp"
#include "Core/Utils/Color.hpp"

#include "Platform/OpenGL/RendererFF.hpp"
#include "Editor/UI/Fontes/MaterialIcons.hpp"

#include "Editor/Windows/UINodeEditor.hpp"
#include "Editor/Windows/UILogger.hpp"

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

namespace sandbox
{
	// ----------------------------------------------------
	// Primitivos básicos, para visualização e testes
	// ----------------------------------------------------
	ecs::Entity CreateBaseEntity(
		ecs::EntityRegistry& er,
		Vector position, Vector scale, float orderZ,
		PrimitiveType primitive, std::vector<utils::VertexInfo> vertices, Color color
	)
	{
		ecs::Entity object = er.CreateEntity();

		// Componente que armazena as transformações do objeto
	    er.AddComponent<ecs::TransformComponent>(
	        object, 
	        position, scale, orderZ
	    );

	    // Componente responsável por enviar comandos para o renderizador
	    er.AddComponent<ecs::ShapeComponent>(
	        object,
	        (uint32_t)0, // Sem textura
	        color, primitive, vertices, orderZ
	    );

	    return object;
	}

	ecs:: Entity CreateRect(ecs::EntityRegistry& er, Color color)
	{
		return CreateBaseEntity(
			er,
			Vector(0.0f, 0.0f), Vector(1.0f, 1.0f), 0.0f,
			PrimitiveType::Quads,
			std::vector<utils::VertexInfo> {
	            { Vector(-0.5f, -0.5f), Vector(0.0f, 0.0f) },
	            { Vector( 0.5f, -0.5f), Vector(0.0f, 0.0f) },
	            { Vector( 0.5f,  0.5f), Vector(0.0f, 0.0f) },
	            { Vector(-0.5f,  0.5f), Vector(0.0f, 0.0f) },
	        }, color
		);
	}

	ecs:: Entity CreateStar(ecs::EntityRegistry& er, int points, Color color)
	{
		std::vector<utils::VertexInfo> vertices;
		float Re = 1.0f, Ri = 0.5f;

	    Vector center(0.0f, 0.0f); // Centro da estrela
	    float angleStep = 3.1415 / points;

	    std::vector<Vector> outer;

	    // Gera os vértices alternando externo e interno
	    for (int i = 0; i < points * 2; i++)
	    {
	        float angle = i * angleStep;
	        float radius = (i % 2 == 0) ? Re : Ri;

	        float x = std::cos(angle) * radius;
	        float y = std::sin(angle) * radius;

	        outer.push_back(Vector(x, y));
	    }

	    // Monta triângulos entre o centro e cada par de vértices
	    for (int i = 0; i < points * 2; ++i)
	    {
	        Vector v1 = outer[i];
	        Vector v2 = outer[(i + 1) % (points * 2)];

	        vertices.push_back({center, center});
	        vertices.push_back({v1, v1});
	        vertices.push_back({v2, v2});
	    }

		return CreateBaseEntity(
			er,
			Vector(0.0f, 0.0f), Vector(1.0f, 1.0f), 0.0f,
			PrimitiveType::Triangles, vertices, color
		);
	}

	ecs:: Entity CreateCircle(ecs::EntityRegistry& er, int vcount, float size, Color color)
	{
		std::vector<utils::VertexInfo> vertices;
		
		for (int v=0; v<=vcount; v++)
	    {
	        float angle = 2.0f * 3.1415 * v / vcount;

	        float x = std::cos(angle) * size;
	        float y = std::sin(angle) * size;

	        vertices.push_back({Vector(x, y), Vector(x, y)});
	        vertices.push_back({Vector(x, y), Vector(x, y)});
	    }

		return CreateBaseEntity(
			er,
			Vector(0.0f, 0.0f), Vector(1.0f, 1.0f), 0.0f,
			PrimitiveType::TriangleFan, vertices, color
		);
	}

	ecs::Entity CreatePlayableEntity(ecs::EntityRegistry& entitiesManager, Color color)
	{
		ecs::Entity playable = CreateRect(entitiesManager, color);

		auto& input = entitiesManager.AddComponent<ecs::InputComponent>(playable);
		auto* transform = entitiesManager.GetComponent<ecs::TransformComponent>(playable);

		input.AddAction("forward", SDLK_w, [transform](double dt){
	        transform->position.y += 10.0f * dt;
	    }, KeyStateEnum::Held);

	    input.AddAction("back", SDLK_s, [transform](double dt){
	        transform->position.y -= 10.0f * dt;
	    }, KeyStateEnum::Held);

	    input.AddAction("left", SDLK_a, [transform](double dt){
	        transform->position.x -= 10.0f * dt;
	    }, KeyStateEnum::Held);

	    input.AddAction("right", SDLK_d, [transform](double dt){
	        transform->position.x += 10.0f * dt;
	    }, KeyStateEnum::Held);

	    return playable;
	}

	void InitialScene(KodeshApplication& app, Renderer& renderer)
	{
	    // Demo (Sandbox)
	    SceneManager* sm = app.GetSystem<SceneManager>();
		sm->AddScene("Scene");
	}

	void UserInterfaceModulation(KodeshApplication& app, UILayer& ui)
	{
	    auto& tests = ui.AddLayoutMenubarItem((std::string(MICON_LABS) + "  Tests [A/B]").c_str());
	    {
	    	auto& add = ui.AddLayoutMenubarItem(tests, "Adicionar", "");
	    	{
	    		ui.AddLayoutMenubarItem(add, "Estrela", "",
	    			[&app](){
	    				// Demo (Sandbox)
	    				SceneManager* sm = app.GetSystem<SceneManager>();
	    				Scene* scn = sm->GetActiveScene();

    					ecs::EntityRegistry& em = scn->GetRegistry();
    					CreateStar(em, 5, Color(1.0f, 1.0f, 0.0f, 1.0f));
	    			}
    			);

	    		ui.AddLayoutMenubarItem(add, "Retangulo", "",
	    			[&app](){
	    				// Demo (Sandbox)
	    				SceneManager* sm = app.GetSystem<SceneManager>();
	    				Scene* scn = sm->GetActiveScene();

    					ecs::EntityRegistry& em = scn->GetRegistry();
    					CreateRect(em, Color(1.0f, 0.0f, 0.0f, 1.0f));
	    			}
    			);

	    		ui.AddLayoutMenubarItem(add, "Circle", "",
	    			[&app](){
	    				// Demo (Sandbox)
	    				SceneManager* sm = app.GetSystem<SceneManager>();
	    				Scene* scn = sm->GetActiveScene();

    					ecs::EntityRegistry& em = scn->GetRegistry();
    					CreateCircle(em, 32, 1.0f, Color(1.0f, 1.0f, 0.0f, 1.0f));
	    			}
    			);

	    		ui.AddLayoutMenubarSeparator(add);
    			ui.AddLayoutMenubarItem(add, "Entidade Interativa", "",
	    			[&app](){
	    				// Demo (Sandbox)
	    				SceneManager* sm = app.GetSystem<SceneManager>();
	    				Scene* scn = sm->GetActiveScene();

    					ecs::EntityRegistry& em = scn->GetRegistry();
    					CreatePlayableEntity(em, Color(1.0f, 1.0f, 1.0f, 1.0f));
	    			}
    			);
	    	}
	    }
	}

	void UserInterfaceWindows(KodeshApplication& app, UILayer& ui)
	{
		auto nodeWin = std::make_unique<editor::nodes::UINodeEditor>(app.GetRenderer());
		auto loggerWin = std::make_unique<editor::windows::UILogger>();

		nodeWin->SpawnOutputActionNode();
		nodeWin->SpawnOutputActionNode();

		ui.AddWindow(std::move(nodeWin)); // retorna um inteiro, o qual é a identificação da janela.
		ui.AddWindow(std::move(loggerWin));
	}
}