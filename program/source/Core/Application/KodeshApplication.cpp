#include <iostream>

#include "Platform/SDL/SDLWindow.hpp"

#include "Core/Events/WindowEvent.hpp"
#include "Core/Events/MouseEvent.hpp"
#include "Core/Events/KeyboardEvent.hpp"
#include "Core/Events/SaveSystemEvent.hpp"
#include "Core/Events/KodeshModeChangedEvent.hpp"
#include "Core/Application/KodeshApplication.hpp"

#include "Core/ECS/TransformComponent.hpp"
#include "Core/ECS/ShapeComponent.hpp"

#include "Core/Helpers/LogManager.hpp"

#include "Core/Serialization/SerializerRegistry/ComponentsRegistry.hpp"

#include <iostream>
#include <fstream>

#include <SDL2/SDL.h>

namespace core
{
	using namespace events;

	namespace app
	{
		// Nome e path padrão do arquivo de configurações.
		const char* KodeshApplication::s_ConfigsPath = "configs.json";

		KodeshApplication::KodeshApplication(): m_Running(true), m_Mode(KodeshModeEnum::EDIT_MODE)
		{
			m_Window = Window::Create();

			m_InputManager = std::make_unique<input::InputManager>();
		}

		KodeshApplication::~KodeshApplication()
		{}

		nlohmann::json KodeshApplication::GetConfigs()
		{
			std::ifstream file(s_ConfigsPath);
			nlohmann::json configs;

			if (file.good())
			{
				file >> configs;
			}

			return configs;
		}

		const char* KodeshApplication::GetConfigsFilepath()
		{
			return s_ConfigsPath;
		}

		void KodeshApplication::RegisterWindowCallbacks()
		{
			EventDispatcher& dispatcher = m_Window->GetDispatcher();

			// Registra o close da janela
			dispatcher.Register<WindowCloseEvent>(
				[&](Event& e){ 
					// Saída da engine.
					if (e.GetEventType() == EventType::WindowClose)
					{
						m_Running = false;
						e.Handled = true;
					}
				}
			);

			// Registra o resize da janela
			dispatcher.Register<WindowResizeEvent>(
				[&](Event& e){ 
					const WindowResizeEvent* we = dynamic_cast<const WindowResizeEvent*>(&e);
					m_Window->SetWidth(we->GetWidth());
					m_Window->SetHeight(we->GetHeight());
				}
			);

			// Registra o teclado para a engine.
			dispatcher.Register<KeyPressedEvent>(
				[&](Event& e){ 
					// Troca de modos.
					if (e.GetEventType() == EventType::KeyPressed)
					{
						const KeyPressedEvent* kpe = dynamic_cast<const KeyPressedEvent*>(&e);

						if (GetMode() == KodeshModeEnum::EDIT_MODE && kpe->GetKeyCode() == SDLK_p)
						{
							KodeshModeChangedEvent event(GetMode(), KodeshModeEnum::PLAY_MODE);
							m_Window->EventDispatch(event);
							e.Handled = true;
						}
						else if (GetMode() == KodeshModeEnum::PLAY_MODE && kpe->GetKeyCode() == SDLK_ESCAPE)
						{
							KodeshModeChangedEvent event(GetMode(), KodeshModeEnum::EDIT_MODE);
							m_Window->EventDispatch(event);
							e.Handled = true;
						}
					}
				}
			);

			// Registra a troca de modos da engine.
			dispatcher.Register<KodeshModeChangedEvent>(
				[&](Event& e){ 
					const KodeshModeChangedEvent* kmc = dynamic_cast<const KodeshModeChangedEvent*>(&e);
					SwitchMode(kmc->GetNewMode());
				}
			);
		}

		void KodeshApplication::RegisterComponentSerializers()
		{
			auto& rcs = serialization::ComponentsRegistry::Instance();

			rcs.RegisterComponent<ecs::TransformComponent>("<TransformComponent>");
			rcs.RegisterComponent<ecs::ShapeComponent>("<ShapeComponent>");
			rcs.RegisterComponent<ecs::TagComponent>("<TagComponent>");
			rcs.RegisterComponent<ecs::LifetimeComponent>("<LifetimeComponent>");
			rcs.RegisterComponent<ecs::InputComponent>("<InputComponent>");
		}

		void KodeshApplication::UseRenderer(std::unique_ptr<renderer::Renderer> r)
		{
			m_Renderer = std::move(r);
		}

		void KodeshApplication::SwitchMode(KodeshModeEnum mode)
		{
			if (m_Mode == mode || /* Ignora caso de igualdade de modos */ 
				mode == KodeshModeEnum::SWAPPING /* Ignora o modo swapping, este é apenas para carregamentos */
			) 
			{
				return;
			}

			if (m_Mode == KodeshModeEnum::EDIT_MODE)
			{
				// Verifica se vai entrar em estado de play (Essa verificação é para caso seja acionado mais estados como um debug para physics)
				if (mode == KodeshModeEnum::PLAY_MODE)
				{
					// Se certifica de apagar o mode antes de iniciar cópias.
					m_Systems.erase(mode);
					m_SystemsLookup.erase(mode);

					// Aqui deverá iniciar o backup dos sistemas que utilizam PLAY_MODE, serão jogados para outra pilha 
					// de sistemas na mesma ordem em que estão, assim é possível iniciar sistemas duas vezes.
					for (auto& subsystem : m_Systems[GetMode()])
					{
						// Impede que sistemas que não podem ser utilizados em determinado modo, não sejam copiados.
						if ((subsystem.Modes & mode) != KodeshModeEnum::NONE)
						{
							std::unique_ptr<systems::ISystem> sys = subsystem.System->GetClone();

							// Insere o sistema em lookup do modo correspondente
							m_SystemsLookup[mode][subsystem.Tidx] = sys.get();

							m_Systems[mode].push_back(SystemManager{
								std::move(sys),
								subsystem.Modes,
								subsystem.Tidx,
								true // Agora ele é temporário pois é uma cópia.
							});
						}
					}

					m_Mode = mode;

					return;
				}
			}
			else if (m_Mode == KodeshModeEnum::PLAY_MODE)
			{
				if (mode == KodeshModeEnum::EDIT_MODE)
				{
					// Aqui os sistemas que foram copiados para execução em play, serão apagados, e o sistema 
					// voltará ao estado original.
					m_Mode = mode;
					return;
				}
			}
		}

		void KodeshApplication::SwapSystems()
		{
			// Verifica se há sistemas pendentes a serem recarregados.
			if (m_Systems.count(KodeshModeEnum::SWAPPING))
			{
				// Economia de custos para diversos sistemas, a ideia é fazer um lookup temporario
				std::unordered_map<std::type_index, size_t> editMap;
				auto& editSystems = m_Systems[KodeshModeEnum::EDIT_MODE];

				for (size_t i = 0; i < editSystems.size(); ++i)
				{
				    editMap[editSystems[i].Tidx] = i;
				}

				// Passa pelo swapping para fazer as trocas.
				for (auto& swapsys : m_Systems[KodeshModeEnum::SWAPPING]) 
				{
				    auto it = editMap.find(swapsys.Tidx);

				    if (it != editMap.end()) 
				    {
				        editSystems[it->second].System = std::move(swapsys.System);
				    }
				}

				// Limpeza
				m_Systems.erase(KodeshModeEnum::SWAPPING);
			}
		}

		void KodeshApplication::Run()
		{
			m_InputManager->RegisterEventsOnDispatcher(m_Window->GetDispatcher());

			while(m_Running)
			{
				// 0.5 Atualiza o delta time.
				m_Timer.Tick();

				// 1. Processa eventos SDL + dispatcher
				m_Window->OnUpdate();

				// 2. Atualiza sistemas
				for (auto& subsystem : m_Systems[GetMode()])
				{
					// Todos os sistemas são adicionados na primeira camada, EDIT_MODE, 
					// mas nem todos serão executados nessa camada, podem ter sistemas que funcionem
					// e apareçam apenas no PLAY_MODE ou em uma camada nova.
					if ((subsystem.Modes & GetMode()) != KodeshModeEnum::NONE)
					{
						subsystem.System->Update(m_Timer.DeltaTime());
					}
				}
				
				// 3. Atualizar o estado dos inputs
				m_InputManager->Update();

				// 4. Passagem por sistemas
				for (auto& subsystem : m_Systems[GetMode()])
				{
					if ((subsystem.Modes & GetMode()) != KodeshModeEnum::NONE)
					{
						// 4.1 Renderização de sistemas.
						subsystem.System->Render(*m_Renderer, 
							m_Window->GetWidth(), m_Window->GetHeight(), 
							m_Timer.DeltaTime()
						);
					}
				}

				// 5. Atualização de sistemas pré carregados em swapping
				SwapSystems();
			}
		}

	}
}