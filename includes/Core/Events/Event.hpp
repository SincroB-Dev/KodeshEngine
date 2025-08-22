#pragma once

#include <string>

namespace core
{
	namespace events
	{
		/**
		 * Tipos de eventos que podem ser filtrados pelo listener 
		 */
		enum class EventType
		{
			None = 0,
			WindowClose, WindowResize,
			KeyPressed, KeyReleased,
			MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
		};

		class Event
		{
		public:
			Event() {}

			virtual ~Event() = default;
			virtual EventType GetEventType() const = 0;
			inline virtual std::string ToString() const { return "Event"; }

			bool Handled = false; // Se true, subsistemas posteriores ignoram.
		};
	}
}