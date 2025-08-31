#include "Core/Systems/SceneManager.hpp"

#include "Core/ECS/Systems/RenderSystem.hpp"
#include "Core/ECS/Systems/InputSystem.hpp"
#include "Core/ECS/Systems/GlobalSystem.hpp"

#include "Core/Maths/Vector.hpp"
#include "Core/Maths/Matrix4x4.hpp"

namespace core
{
	using namespace renderer;
	using namespace mathutils;
	using namespace scene;

	namespace systems
	{
		SceneManager::SceneManager(events::EventDispatcher& dispatcher, input::InputManager& input)
			: ka_InputManager(input)
		{
			// Registrar callbacks se necessário;
		}

		Scene* SceneManager::AddScene(const std::string& name)
		{
			auto scn = std::make_unique<Scene>(name);

    		m_ActiveScene = scn.get();
			
			// Adiciona o sistema de renderização da cena
    		scn->AddRenderSystem(std::make_unique<ecs::RenderSystem>(GetRenderQueue()));

    		// Adiciona outros sistemas a cena
    		scn->AddSystem(std::make_unique<ecs::GlobalSystem>());
    		scn->AddSystem(std::make_unique<ecs::InputSystem>(GetInputManager()));

			m_Scenes.push_back(std::move(scn));

			return m_ActiveScene;
		}

		Scene* SceneManager::GetScene(const std::string& name)
		{
			auto it = std::find_if(m_Scenes.begin(), m_Scenes.end(),
		        [&name](const std::unique_ptr<Scene>& scn) {
		            return scn->GetName() == name;
		        });

		    if (it != m_Scenes.end() && *it)
		    {
		        return it->get();
		    }

		    return nullptr;
		}

		Scene* SceneManager::GetActiveScene()
		{
			return m_ActiveScene;
		}

		void SceneManager::RemoveScene(const std::string& name)
		{
		    auto it = std::find_if(m_Scenes.begin(), m_Scenes.end(),
		        [&name](const std::unique_ptr<Scene>& scn) {
		            return scn->GetName() == name;
		        });

		    if (it != m_Scenes.end())
		    {
		        // Atualiza a cena ativa se necessário
		        if (m_ActiveScene != nullptr && m_ActiveScene->GetName() == name)
		        {
		            m_ActiveScene = (m_Scenes.size() > 1) ? m_Scenes.back().get() : nullptr;
		        }

		        m_Scenes.erase(it);
		    }
		}

		void SceneManager::Update(double deltaTime)
		{
			// Atualização das entidades em cena
			if (m_ActiveScene)
			{
				m_ActiveScene->Update(deltaTime);
			}
		}

		void SceneManager::Render(Renderer& renderer, int w, int h, double deltaTime)
		{
			// Projeção ortografica
			RendererFrameInfo info;

			info.ViewportW = w; info.ViewportH = h;
			info.ViewProjection = Matrix4x4::OrthoAspect(10.0f, w, h);

			renderer.BeginFrame(info);

			m_RenderQueue.Clear();

			if (m_ActiveScene)
			{
				m_ActiveScene->Render(renderer, deltaTime);
			}

			renderer.Flush(m_RenderQueue);

			renderer.EndFrame();
		}
	}
}