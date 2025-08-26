#pragma once

#include <sstream>

#include "Core/Events/Event.hpp"

namespace core
{
	namespace events
	{
		class KeyPressedEvent : public Event
		{
		public:
			KeyPressedEvent(int keycode, bool repeat)
				: m_KeyCode(keycode), m_Repeat(repeat) {}

			inline int GetKeyCode() const { return m_KeyCode; }

			EventType GetEventType() const override { return EventType::KeyPressed; }
			std::string ToString() const override
			{
				std::stringstream ss;
				ss << "<KeyPressedEvent: " << m_KeyCode << ", repeat: " << m_Repeat << ">";
				return ss.str();
			}

		private:
			const int m_KeyCode;
			const bool m_Repeat;
		};

		class KeyReleasedEvent : public Event
		{
		public:
			KeyReleasedEvent(int keycode)
				: m_KeyCode(keycode) {}
			
			inline int GetKeyCode() const { return m_KeyCode; }

			EventType GetEventType() const override { return EventType::KeyReleased; }
			std::string ToString() const override
			{
				std::stringstream ss;
				ss << "<KeyReleasedEvent: " << m_KeyCode << ">";
				return ss.str();
			}

		private:
			const int m_KeyCode;
		};
	}
}