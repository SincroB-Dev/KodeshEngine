#pragma once

#include "Core/Application/Window.hpp"
#include "Core/Events/Event.hpp"

#include "Core/Input/InputManager.hpp"

#include "Core/Systems/ISystem.hpp"
#include "Core/Utils/Timer.hpp"

#include <memory>
#include <vector>
#include <typeindex>
#include <functional>
#include <any>
#include <tuple>
#include <nlohmann/json.hpp>

#include <SDL2/SDL.h>
#include "Platform/SDL/SDLWindow.hpp"

#include "Core/Events/KodeshModeChangedEvent.hpp"

#include "Core/Systems/SceneManager.hpp"

namespace core
{
	namespace app
	{
		/**
		 * @brief Encapsulamento de sistemas, assim é possível adicionar sistemas e dizer a qual parte da engine eles pertencem.
		 *        Na troca de modos, é sempre feito um backup, ao sair de um modo, se este for do tipo backup, será destruido 
		 * 		  e removido da pool de sistemas. 
		 **/
		struct SystemManager
		{
			std::unique_ptr<systems::ISystem> System; // Sistema em sí.
			events::KodeshModeEnum Modes; // Modos que o sistema é executado.
			std::type_index Tidx; // Index do tipo de sistema.
			bool IsTemporary; // Indica ao gerenciador se este é um sistema temporário (deve ser apagado quando não estiver em execução), ou, fixo.
		};

		/**
		 * @brief Principal da engine, o qual controla sistemas e gerencia o que pode ou não ser executado. 
		 **/
		class KodeshApplication
		{
		public:
			KodeshApplication();
			virtual ~KodeshApplication();

			void Run();

			Window& GetWindow() { return *m_Window.get(); }

			template<typename T>
			T* GetSystem();

			template<typename T, typename... Args>
			T* RegisterSystem(events::KodeshModeEnum modes, Args&&... args);

			void RegisterWindowCallbacks();
			void RegisterComponentSerializers();

			void UseRenderer(std::unique_ptr<renderer::Renderer> r);

			renderer::Renderer& GetRenderer() { return *m_Renderer.get(); }
			input::InputManager& GetInputManager() { return *m_InputManager.get(); }

			const utils::Timer GetTimer() const { return m_Timer; }

			const events::KodeshModeEnum GetMode() const { return m_Mode; }

			/**
			 * @brief Altera o modo da engine. (por baixo dos panos, a grosso modo, ele é o principal responsável por
			 * 		  abrir cópias de itens que serão alterados durante plays e deverão retornar ao estado original) 
			 **/
			void SwitchMode(events::KodeshModeEnum mode);

			inline void* GetNativeWindow()
			{
				auto* native = dynamic_cast<platform::SDLWindow*>(m_Window.get());
				return native->GetNative();
			}

			inline void* GetContext()
			{
				auto* native = dynamic_cast<platform::SDLWindow*>(m_Window.get());
				return native->GetContext();
			}

			static nlohmann::json GetConfigs();
			static const char* GetConfigsFilepath();

		private:
			/**
			 * @brief Tira sistemas carregados em swap, e os coloca no modo de edição, chamado no fim de cada frame
			 *        foi pensado para garantir que ninguém esteja usando algum sistema durante sua recarga.
			 **/
			void SwapSystems();

		private:
			std::unique_ptr<Window> m_Window;

			// Renderizador
			std::unique_ptr<renderer::Renderer> m_Renderer;
			std::unique_ptr<input::InputManager> m_InputManager;

			// Subsystems (UILayerManager, SceneManager, AudioManager...)
			std::unordered_map<events::KodeshModeEnum, std::vector<SystemManager>> m_Systems;
			std::unordered_map<events::KodeshModeEnum, std::unordered_map<std::type_index, systems::ISystem*>> m_SystemsLookup;

			bool m_Running;
			utils::Timer m_Timer;

			events::KodeshModeEnum m_Mode;

			static const char* s_ConfigsPath;
		};

		template<typename T, typename... Args>
		T* KodeshApplication::RegisterSystem(events::KodeshModeEnum modes, Args&&... args)
		{
			// Dispatcher de eventos, deve ser passado a systems para que estes possam ouvir eventos
    		events::EventDispatcher& dispatcher = GetWindow().GetDispatcher();

    		// Controle de entradas de paralelos
    		input::InputManager& input = GetInputManager();

			std::unique_ptr<T> subsys = std::make_unique<T>(dispatcher, input, std::forward<Args>(args)...);
			
			T* subsysPtr = subsys.get();

			auto& pool = m_Systems[events::KodeshModeEnum::EDIT_MODE];

			pool.push_back(
				SystemManager {
					std::move(subsys),
					modes, // Modos em que o sistema pode existir. (Não é feito aqui o controle do que pode ser executado...)
					typeid(T), // Tipo do sistema para utilização futura.
					false // Sistemas registrados nunca devem ser temporários, essa flag apenas é modificada em atos de cópia para novos modos.
				}
			);
	
			m_SystemsLookup[events::KodeshModeEnum::EDIT_MODE][typeid(T)] = subsysPtr;

			return subsysPtr;
		}

		template<typename T>
		T* KodeshApplication::GetSystem()
		{
			auto it = m_SystemsLookup[GetMode()].find(typeid(T));
			if (it != m_SystemsLookup[GetMode()].end())
			{
				return static_cast<T*>(it->second);
			}
			return nullptr;
		}
	}
}