#pragma once

#include "Core/Events/EventDispatcher.hpp"
#include "Core/Events/Event.hpp"
#include "Core/Input/InputManager.hpp"

#include "Core/Systems/ISystem.hpp"

#include "Core/Scene/Scene.hpp"

#include "Core/Renderer/RenderQueue.hpp"
#include "Core/Renderer/Renderer.hpp"

#include "Core/Serialization/PersistenceFwd.hpp"

#include <memory>
#include <nlohmann/json.hpp>

namespace core
{
	namespace systems
	{
		/**
		 * Resposável por gerenciar todas as cenas e suas entidades
		 */
		class SceneManager : public ISystem
		{
		public:
			SceneManager(events::EventDispatcher& dispatcher, input::InputManager& input);
			SceneManager(input::InputManager& input); // Construtor sem inclusão de callbacks de eventos. ()

			scene::Scene* AddScene(const std::string& name);
			scene::Scene* GetScene(const std::string& name);
			scene::Scene* GetActiveScene();

			void RemoveScene(const std::string& name);

			void Update(double deltaTime) override;
			void Render(renderer::Renderer& renderer, int w, int h, double deltaTime) override;

			/**
			 * @brief Retorna um clone funcional do scene manager, como scenes serão construídas
			 * 		  e seus sistemas não serão executados, estes serão dispachados para serem
			 * 		  executados no clone.
			 **/
			std::unique_ptr<ISystem> GetClone() override;

			renderer::RenderQueue& GetRenderQueue() { return m_RenderQueue; }
			input::InputManager& GetInputManager() { return ka_InputManager; }

		private:
			// Cena ativa e coleção
			scene::Scene* m_ActiveScene;
			
			std::vector<std::unique_ptr<scene::Scene>> m_Scenes;

			// Fila de renderização
			renderer::RenderQueue m_RenderQueue;

			// Apontamento para o gerenciador de inputs (ka_ indica que vem do core, KodeshApplication)
			input::InputManager& ka_InputManager;

			// Friendship com serializadores/deserializadores
	    	friend nlohmann::json serialization::persistence::SerializeSystem<SceneManager>(const SceneManager&);
		};
	}
}