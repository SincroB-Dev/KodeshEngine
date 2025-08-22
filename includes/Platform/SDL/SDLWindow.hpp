#pragma once

#include <SDL2/SDL.h>

#include "Core/Application/Window.hpp"

namespace platform
{
	class SDLWindow : public core::app::Window
	{
	public:
		SDLWindow(const core::app::WindowProps& props);
		~SDLWindow();

		void OnUpdate() override;

		unsigned int GetWidth() const override;
		unsigned int GetHeight() const override;

		void SetWidth(int width) override;
		void SetHeight(int height) override;

	private:
		void HandleEvents(const SDL_Event& e);

	private:
		SDL_Window* m_Window;
		SDL_GLContext m_Context;

		unsigned int m_Width, m_Height;
	};
}

// Factory
inline std::unique_ptr<core::app::Window> core::app::Window::Create(const core::app::WindowProps& props)
{
	return std::make_unique<platform::SDLWindow>(props);
}