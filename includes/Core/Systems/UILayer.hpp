#pragma once

#include "Core/Events/EventDispatcher.hpp"
#include "Core/Events/Event.hpp"
#include "Core/Input/InputManager.hpp"

#include "Core/Systems/ISystem.hpp"

#include "Core/Renderer/Renderer.hpp"

namespace core
{
	namespace systems
	{
		/**
		 * Camada de UI consome os eventos primeiro. 
		 */
		class UILayer : public ISystem
		{
		public:
			UILayer(events::EventDispatcher& dispatcher, input::InputManager& input);

			void OnEvent(events::Event& e) override;
			
			void Update(double deltaTime) override;
			void Render(renderer::Renderer& renderer, int w, int h, double deltaTime) override;
		};
	}
}