#pragma once

#include "Core/Maths/Transform2D.hpp"

#include "Core/Renderer/RenderQueue.hpp"
#include "Core/Renderer/Renderer.hpp"

#include "Core/ECS/EntityRegistry.hpp"
#include "Core/ECS/Systems/ISystem.hpp"

#include "Core/ECS/TransformComponent.hpp"
#include "Core/ECS/ShapeComponent.hpp"

#include <vector>
#include <memory>

namespace core
{
	namespace scene
	{
		class Scene
		{
		public:
			Scene(const std::string& name): m_Name(name) {}
			~Scene() {}

			inline ecs::EntityRegistry& GetRegistry() { return m_Registry; }
			inline std::string GetName() { return m_Name; }
			inline void SetName(const std::string& name) { m_Name = name; }

			void Update(double deltaTime);
			void Render(renderer::Renderer& renderer, double deltaTime);

			void AddSystem(std::unique_ptr<ecs::ISystem> system);
			void AddRenderSystem(std::unique_ptr<ecs::ISystem> renderSystem);

		private:
			std::string m_Name;

			ecs::EntityRegistry m_Registry;

			std::vector<std::unique_ptr<ecs::ISystem>> m_Systems;
			std::vector<std::unique_ptr<ecs::ISystem>> m_RenderSystems;
		};
	}
}