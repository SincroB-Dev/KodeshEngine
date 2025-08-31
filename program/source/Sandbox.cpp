#include "Sandbox.hpp"

#include <math.h>

#include "Core/Application/KodeshApplication.hpp"

#include "Core/Systems/SceneManager.hpp"
#include "Core/Systems/UILayerManager.hpp"
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

#include "Core/Helpers/LogManager.hpp"
#include "Core/Renderer/RenderCommand.hpp"

#include "Core/Maths/Vector.hpp"
#include "Core/Maths/Color.hpp"

#include "Platform/OpenGL/RendererFF.hpp"
#include "Editor/UI/Fontes/MaterialIcons.hpp"

#include "Editor/Windows/UINodeEditor.hpp"
#include "Editor/Windows/UINodeEditor/Compositor.hpp"
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

using namespace editor;
using namespace platform;

namespace comp = nodes::compositor;

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
		LogManager::Log(LogType::Info, "Criando Entity...");
		ecs::Entity object = er.CreateEntity();

		LogManager::Log(LogType::Info, "Anexando componente <TransformComponent>...");
		// Componente que armazena as transformações do objeto
	    er.AddComponent<ecs::TransformComponent>(
	        object, 
	        position, scale, orderZ
	    );

		LogManager::Log(LogType::Info, "Anexando componente <ShapeComponent>...");
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
		LogManager::Log(LogType::Info, "Preparando para criar Rect...");
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
		LogManager::Log(LogType::Info, "Preparando para criar Estrela...");
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
		LogManager::Log(LogType::Info, "Preparando para criar Circulo...");
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
		LogManager::Log(LogType::Info, "Iniciando Entity Interativa...");
		ecs::Entity playable = CreateRect(entitiesManager, color);

		LogManager::Log(LogType::Info, "Anexando componente <InputComponent>...");
		auto& input = entitiesManager.AddComponent<ecs::InputComponent>(playable);
		auto* transform = entitiesManager.GetComponent<ecs::TransformComponent>(playable);

		LogManager::Log(LogType::Info, "Anexando actions no <InputComponent>...");
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

	    std::cout << SDLK_w << std::endl;
	    std::cout << SDLK_a << std::endl;
	    std::cout << SDLK_s << std::endl;
	    std::cout << SDLK_d << std::endl;

	    return playable;
	}

	void InitialScene(KodeshApplication& app, Renderer& renderer)
	{
	    // Demo (Sandbox)
	    SceneManager* sm = app.GetSystem<SceneManager>();

		LogManager::Log(LogType::Info, "Criando cena inicial {Scene}...");
		sm->AddScene("Scene");
	}

	void UserInterfaceModulation(KodeshApplication& app, UILayerManager& ui)
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

	void UserInterfaceWindows(KodeshApplication& app, UILayerManager& ui)
	{
		LogManager::Log(LogType::Info, "Construindo <UINodeEditor>...");
		auto nodeWin = std::make_unique<nodes::UINodeEditor>(app.GetRenderer());
		// Construção dos nodes.
		{
			// Conversão de teclas na mão por enquanto, teclas + (magic number 449)
			// sendo a distancia fixa de SDLK para ImGuiKeyCode.
			auto onUpdate = comp::Compositor::OnUpdateNode(app, nodeWin.get());

			auto inputW = comp::Compositor::InputEventNode(app, nodeWin.get(), SDLK_w + 449);
			auto inputA = comp::Compositor::InputEventNode(app, nodeWin.get(), SDLK_a + 449);
			auto inputS = comp::Compositor::InputEventNode(app, nodeWin.get(), SDLK_s + 449);
			auto inputD = comp::Compositor::InputEventNode(app, nodeWin.get(), SDLK_d + 449);
			
			comp::Compositor::Connect(nodeWin.get(), onUpdate->Outputs[0].get(), inputW->Inputs[0].get());
			comp::Compositor::Connect(nodeWin.get(), onUpdate->Outputs[0].get(), inputA->Inputs[0].get());
			comp::Compositor::Connect(nodeWin.get(), onUpdate->Outputs[0].get(), inputS->Inputs[0].get());
			comp::Compositor::Connect(nodeWin.get(), onUpdate->Outputs[0].get(), inputD->Inputs[0].get());
		}
		ui.AddWindow(std::move(nodeWin));

		LogManager::Log(LogType::Info, "Construindo <UILogger>...");
		auto loggerWin = std::make_unique<windows::UILogger>();
		ui.AddWindow(std::move(loggerWin));
	}
}