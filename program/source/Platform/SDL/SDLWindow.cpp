#include "Platform/SDL/SDLWindow.hpp"

#include <GL/gl.h>
#include <memory>

#include "Core/Events/KeyboardEvent.hpp"
#include "Core/Events/WindowEvent.hpp"
#include "Core/Events/MouseEvent.hpp"

namespace platform
{
	using namespace core::events;

	SDLWindow::SDLWindow(const core::app::WindowProps& props)
	{
		SDL_Init(SDL_INIT_VIDEO);

		m_Window = SDL_CreateWindow(
			props.Title.c_str(),
			SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
			props.Width, props.Height,
			SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE
		);

		m_Context = SDL_GL_CreateContext(m_Window);

		SetWidth(props.Width);
		SetHeight(props.Height);
	}

	SDLWindow::~SDLWindow()
	{
		SDL_GL_DeleteContext(m_Context);
		SDL_DestroyWindow(m_Window);
		SDL_Quit();
	}

	void SDLWindow::OnUpdate()
	{
		SDL_Event e;
		while (SDL_PollEvent(&e))
		{
			HandleEvents(e);
		}
		SDL_GL_SwapWindow(m_Window);
	}

	unsigned int SDLWindow::GetWidth() const
	{
		return m_Width;
	}

	unsigned int SDLWindow::GetHeight() const
	{
		return m_Height;
	}

	void SDLWindow::SetWidth(int width)
	{
		m_Width = width;
	}
	
	void SDLWindow::SetHeight(int height)
	{
		m_Height = height;
	}

	void SDLWindow::HandleEvents(const SDL_Event& e)
	{
		switch (e.type)
		{
		case SDL_QUIT:
			{
				WindowCloseEvent event;
				event.NativeEvent = &e;
				EventDispatch(event);
			}
			break;

		case SDL_WINDOWEVENT:
			if (e.window.event == SDL_WINDOWEVENT_RESIZED)
			{
				WindowResizeEvent event(e.window.data1, e.window.data2);
				event.NativeEvent = &e;
				EventDispatch(event);
			}
			break;

		case SDL_KEYDOWN:
			{
				bool repeat = e.key.repeat != 0; 
				KeyPressedEvent event(e.key.keysym.sym, repeat);
				event.NativeEvent = &e;
				EventDispatch(event);
			}	
			break;

		case SDL_KEYUP:
			{
				KeyReleasedEvent event(e.key.keysym.sym);
				event.NativeEvent = &e;
				EventDispatch(event);
			}
			break;

		case SDL_MOUSEMOTION:
			{
				MouseMovedEvent event((float)e.motion.x, (float)e.motion.y);
				event.NativeEvent = &e;
				EventDispatch(event);
			}
			break;

		case SDL_MOUSEBUTTONDOWN:
			{
				MouseButtonPressedEvent event(e.button.button);
				event.NativeEvent = &e;
				EventDispatch(event);
			}
			break;

		case SDL_MOUSEBUTTONUP:
			{
				MouseButtonReleasedEvent event(e.button.button);
				event.NativeEvent = &e;
				EventDispatch(event);
			}
			break;

		case SDL_MOUSEWHEEL:
			{
				MouseScrolledEvent event((float)e.wheel.x, (float)e.wheel.y);
				event.NativeEvent = &e;
				EventDispatch(event);
			}
			break;
		}
	}
}