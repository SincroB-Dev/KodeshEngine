#pragma once

#include "Core/Application/KodeshApplication.hpp"

#include "Core/Systems/SceneManager.hpp"
#include "Core/Systems/UILayerManager.hpp"

#include "Core/Renderer/Renderer.hpp"

#include "Core/ECS/EntityRegistry.hpp"

using namespace core::events;
using namespace core::systems;
using namespace core::scene;
using namespace core::mathutils;
using namespace core::utils;
using namespace core::renderer;
using namespace core::input;
using namespace core::app;
using namespace core;

using namespace platform;

/**
 * Protocolo bicicleta de rodinha heuhuehuehue
 */
namespace sandbox
{
	/**
	 * @brief Cria uma entidade inicial com componentes pré estabelecidos. 
	 * 
	 * ---------------------------------------------------------
	 *                   [Demonstração]
	 * ---------------------------------------------------------
		CreateEntity(
			Vector(0.0f, 0.0f), Vector(1.0f, 1.0f), 0.0f,
			PrimitiveType::Quads,
			std::vector<utils::VertexInfo> {
	            { Vector(-0.5f, -0.5f), Vector(0.0f, 0.0f) },
	            { Vector( 0.5f, -0.5f), Vector(0.0f, 0.0f) },
	            { Vector( 0.5f,  0.5f), Vector(0.0f, 0.0f) },
	            { Vector(-0.5f,  0.5f), Vector(0.0f, 0.0f) },
	        }, Color(1.0f, 1.0f, 1.0f, 1.0f)
		)
	 *
	 * @return Entity criada com componentes iniciais.
	 **/
	ecs::Entity CreateBaseEntity(
		ecs::EntityRegistry& entitiesManager,
		// TransformationComponent
		Vector position, Vector scale, float orderZ,
		// ShapeComponent
		PrimitiveType primitive, std::vector<utils::VertexInfo> vertices, Color color
	);

	/**
	 * @brief Criação de retangulo
	 **/
	ecs::Entity CreateRect(ecs::EntityRegistry& entitiesManager, Color color);

	/**
	 * @brief Criação de estrela
	 **/
	ecs::Entity CreateStar(ecs::EntityRegistry& entitiesManager, int points, Color color);

	/**
	 * @brief Criação de circulo
	 **/
	ecs::Entity CreateCricle(ecs::EntityRegistry& entitiesManager, int vcount, float size, Color color);

	/**
	 * @brief Cria um componente (retangulo) que pode se mover usando W,A,S,D 
	 **/
	ecs::Entity CreatePlayableEntity(ecs::EntityRegistry& entitiesManager, Color color);
	
	/**
	 * @brief Criação de interfaces para testes de usabilidade da engine.
	 **/
	void UserInterfaceModulation(KodeshApplication& app, UILayerManager& ui);

	/**
	 * @brief Criação de interfaces do tipo janelas complexas, para testar ferramentas da engine.
	 **/
	void UserInterfaceWindows(KodeshApplication& app, UILayerManager& ui);

	/**
	 * @brief Cria uma configuração inicial de cena, no sistema de gerenciamento de cenas
	 **/
	void InitialScene(KodeshApplication& app, Renderer& renderer);

}