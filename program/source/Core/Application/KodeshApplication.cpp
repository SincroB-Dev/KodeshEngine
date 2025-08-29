#include <iostream>

#include "Platform/SDL/SDLWindow.hpp"

#include "Core/Events/WindowEvent.hpp"
#include "Core/Events/MouseEvent.hpp"
#include "Core/Events/KeyboardEvent.hpp"
#include "Core/Application/KodeshApplication.hpp"

#include <iostream>
#include <fstream>

namespace core
{
	using namespace events;

	namespace app
	{
		// Nome e path padrão do arquivo de configurações.
		const char* KodeshApplication::s_ConfigsPath = "configs.json";

		KodeshApplication::KodeshApplication(): m_Running(true)
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
					this->OnEvent(e); 
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
		}

		void KodeshApplication::UseRenderer(std::unique_ptr<renderer::Renderer> r)
		{
			m_Renderer = std::move(r);
		}
		
		void KodeshApplication::OnEvent(Event& e)
		{
			// Ignora eventos já consumidos.
			for (auto& subsystem : m_Systems)
			{
				if (!e.Handled)
				{
					break;
				}

				subsystem->OnEvent(e);
			}

			if (e.GetEventType() == EventType::WindowClose)
			{
				m_Running = false;
				e.Handled = true;
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
				for (auto& system : m_Systems)
				{
					system->Update(m_Timer.DeltaTime());
				}
				
				// 3. Atualizar o estado dos inputs
				m_InputManager->Update();

				// 4. Passagem por sistemas
				for (auto& system : m_Systems)
				{
					// 4.1 Renderização de sistemas.
					system->Render(*m_Renderer, 
						m_Window->GetWidth(), m_Window->GetHeight(), 
						m_Timer.DeltaTime()
					);
				}
			}
		}

	}
}