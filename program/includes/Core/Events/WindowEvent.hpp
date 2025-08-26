#pragma once

#include "Core/Events/Event.hpp"
#include <sstream>

namespace core::events
{
	/**
	 * @brie Evento responsável pela captura de resize da janela.
	 */
	class WindowResizeEvent : public Event
	{
	public:
		inline WindowResizeEvent(int w, int h) : m_Width(w), m_Height(h) {}

		inline int GetWidth() const { return m_Width; };
		inline int GetHeight() const { return m_Height; };

		inline EventType GetEventType() const override { return EventType::WindowResize; };
		inline std::string ToString() const override
		{
			std::stringstream ss;
			ss << "<WindowResizeEvent: " << m_Width << ", " << m_Height << ">";
			return ss.str();
		}

	private:
		int m_Width, m_Height;
	};

	/**
	 * @brief Evento responsável pela captura de chamada de encerramento da janela.
	 */
	class WindowCloseEvent : public Event
	{
	public:
		inline EventType GetEventType() const override { return EventType::WindowClose; }
		inline std::string ToString() const override { return "<WindowCloseEvent>"; }
	};
}