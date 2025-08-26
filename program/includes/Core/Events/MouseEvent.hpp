#pragma once

#include <sstream>

#include "Core/Events/Event.hpp"

namespace core
{
	namespace events
	{
		/**
		 * Evento para captura da posição do mouse
		 */
		class MouseMovedEvent : public Event
		{
		public:
			MouseMovedEvent(float x, float y): m_MouseX(x), m_MouseY(y) {}

			inline float GetX() const { return m_MouseX; }
			inline float GetY() const { return m_MouseY; }

			inline EventType GetEventType() const override { return EventType::MouseMoved; }
			inline std::string ToString() const override
			{
				std::stringstream ss;
				ss << "<MouseMovedEvent: " << m_MouseX << ", " << m_MouseY << ">";
				return ss.str();
			} 

		private:
			float m_MouseX, m_MouseY;
		};

		/**
		 * Evento que trata do press do botão do mouse
		 */
		class MouseButtonPressedEvent : public Event
		{
		public:
			MouseButtonPressedEvent(int button) : m_Button(button) {}

			inline int GetButton() const { return m_Button; }

			inline EventType GetEventType() const override { return EventType::MouseButtonPressed; }
			inline std::string ToString() const override
			{
				std::stringstream ss;
				ss << "<MouseButtonPressed: " << m_Button << ">";
				return ss.str();
			}

		private:
			int m_Button;
		};

		/**
		 * Evento que trata do release do botão do mouse
		 */
		class MouseButtonReleasedEvent : public Event
		{
		public:
			MouseButtonReleasedEvent(int button) : m_Button(button) {}

			inline int GetButton() const { return m_Button; }

			inline EventType GetEventType() const override { return EventType::MouseButtonReleased; }
			inline std::string ToString() const override
			{
				std::stringstream ss;
				ss << "<MouseButtonReleasedEvent: " << m_Button << ">";
				return ss.str();
			}

		private:
			int m_Button;
		};

		/**
		 * Evento para captura da posição do mouse
		 */
		class MouseScrolledEvent : public Event
		{
		public:
			MouseScrolledEvent(float x, float y): m_XOffset(x), m_YOffset(y) {}

			inline float GetXOffset() const { return m_XOffset; }
			inline float GetYOffset() const { return m_YOffset; }

			inline EventType GetEventType() const override { return EventType::MouseScrolled; }
			inline std::string ToString() const override
			{
				std::stringstream ss;
				ss << "<MouseScrolledEvent: " << m_XOffset << ", " << m_YOffset << ">";
				return ss.str();
			} 

		private:
			float m_XOffset, m_YOffset;
		};
	}
}