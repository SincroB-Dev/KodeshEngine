#pragma once

#include <string>
#include <functional>
#include <memory>

#include "Core/Events/Event.hpp"
#include "Core/Events/EventDispatcher.hpp"

namespace core
{
	namespace app
	{
		// Propriedades básicas da janela.
		struct WindowProps
		{
			std::string Title;
			unsigned int Width;
			unsigned int Height;

			WindowProps(const std::string& Title = "Kodesh Engine v1.2",
						unsigned int Width=950, unsigned int Height=500)
				: Title(Title), Width(Width), Height(Height) {}
		};

		class Window
		{
		public:
			using EventCallbackFn = std::function<void(events::Event&)>;

			virtual ~Window() {};

			virtual void OnUpdate() = 0; // troca buffers + processa eventos
			
			virtual unsigned int GetWidth() const = 0;
			virtual unsigned int GetHeight() const = 0;

			virtual void SetWidth(int width) = 0;
			virtual void SetHeight(int height) = 0;

			inline virtual events::EventDispatcher& GetDispatcher() final { return m_Dispatcher; }
			inline virtual void EventDispatch(events::Event& event) final { m_Dispatcher.Dispatch(event); }

			static std::unique_ptr<Window> Create(const WindowProps& props = WindowProps());

		private:
			events::EventDispatcher m_Dispatcher;
		};
	}
}