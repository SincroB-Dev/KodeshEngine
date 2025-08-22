#include "Core/Systems/UILayer.hpp"

namespace core
{
	namespace systems
	{
		UILayer::UILayer(events::EventDispatcher& dispatcher, input::InputManager& input)
		{
			// Registrar callbacks se necessário;
		}

		void UILayer::OnEvent(events::Event& e)
		{
			// Consome eventos e impede que Scene os receba
			/*
			if (e.GetEventType() == events::EventType::MouseButtonPressed)
			{
				std::cout << "[UI] Consumido evento de mouse" << std::endl;
				e.Handled = true; // Impede que a cena receba o evento.
			}
			*/
		}

		void UILayer::Update(double deltaTime)
		{
			// Atualização de lógica da UI
		}

		void UILayer::Render(renderer::Renderer& renderer, int w, int h, double deltaTime)
		{
			// Renderiza ImGui ou outra UI
		}
	}
}