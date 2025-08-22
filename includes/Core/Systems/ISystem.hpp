#pragma once

#include "Core/Events/Event.hpp"
#include "Core/Renderer/Renderer.hpp"

namespace core
{
	namespace systems
	{
		class ISystem
		{
		public:
			virtual ~ISystem() = default;

			virtual void OnEvent(events::Event& e) {} // Fluxo de eventos, opcional;

			virtual void Update(double deltaTime) = 0; // Aplicação da lógica
			virtual void Render(renderer::Renderer& renderer, int w, int h, double deltaTime) = 0; // Aplicação de desenhos
		};
	}
}