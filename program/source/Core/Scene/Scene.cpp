#include <Core/Scene/Scene.hpp>

namespace core
{
	using namespace renderer;
	using namespace ecs;

	namespace scene
	{
		void Scene::Update(double deltaTime)
		{
			for (auto& system : m_Systems)
			{
				system->Update(m_Registry, deltaTime);
			}
		}
		void Scene::Render(Renderer& renderer, double deltaTime)
		{
			for (auto& render : m_RenderSystems)
			{
				render->Update(m_Registry, deltaTime);
			}
		}

		void Scene::AddSystem(std::unique_ptr<ISystem> system)
		{
			m_Systems.push_back(std::move(system));
		}

		void Scene::AddRenderSystem(std::unique_ptr<ISystem> renderSystem)
		{
			m_RenderSystems.push_back(std::move(renderSystem));
		}
	}
}