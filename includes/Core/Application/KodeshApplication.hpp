#pragma once

#include "Core/Application/Window.hpp"
#include "Core/Events/Event.hpp"

#include "Core/Input/InputManager.hpp"

#include "Core/Systems/ISystem.hpp"
#include "Core/Utils/Timer.hpp"

#include <memory>
#include <vector>

namespace core
{
	namespace app
	{
		class KodeshApplication
		{
		public:
			KodeshApplication();
			virtual ~KodeshApplication();

			void Run();
			void OnEvent(events::Event& e);

			Window& GetWindow() { return *m_Window.get(); }

			template<typename T>
			T* GetSystem();

			template<typename T, typename... Args>
			T* RegisterSystem(Args&&... args);

			void RegisterWindowCallbacks();

			void UseRenderer(std::unique_ptr<renderer::Renderer> r);

			renderer::Renderer& GetRenderer() { return *m_Renderer.get(); }
			input::InputManager& GetInputManager() { return *m_InputManager.get(); }

			const utils::Timer GetTimer() const { return m_Timer; }

		private:
			std::unique_ptr<Window> m_Window;

			// Renderizador
			std::unique_ptr<renderer::Renderer> m_Renderer;

			std::unique_ptr<input::InputManager> m_InputManager;

			// Subsystems (UILayer, SceneManager, AudioManager...)
			std::vector<std::unique_ptr<systems::ISystem>> m_Systems;
			std::unordered_map<std::type_index, systems::ISystem*> m_SystemsLookup;

			bool m_Running;
			utils::Timer m_Timer;
		};


		template<typename T, typename... Args>
		T* KodeshApplication::RegisterSystem(Args&&... args)
		{
			// Dispatcher de eventos, deve ser passado a systems para que estes possam ouvir eventos
    		events::EventDispatcher& dispatcher = GetWindow().GetDispatcher();

    		// Controle de entradas de paralelos
    		input::InputManager& input = GetInputManager();

			std::unique_ptr<T> subsys = std::make_unique<T>(dispatcher, input, std::forward<Args>(args)...);
			
			T* subsysPtr = subsys.get();
			m_Systems.push_back(std::move(subsys));
			m_SystemsLookup[typeid(T)] = subsysPtr;

			return subsysPtr;
		}

		template<typename T>
		T* KodeshApplication::GetSystem()
		{
			auto it = m_SystemsLookup.find(typeid(T));
			if (it != m_SystemsLookup.end())
			{
				return static_cast<T*>(it->second);
			}
			return nullptr;
		}
	}
}