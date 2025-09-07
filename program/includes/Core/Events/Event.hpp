#pragma once

#include <string>

namespace core::events
{
	/**
	 * @brief Tipos de eventos que podem ser filtrados pelo listener 
	 */
	enum class EventType
	{
		None = 0,
		WindowClose, WindowResize,
		KeyPressed, KeyReleased, TextInput,
		MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled,
		KodeshModeChanged,
		SaveProject, LoadProject
	};

	/**
	 * @brief Classe base para eventos, sua implementação em eventos é crucial para
	 *        a compatibilidade com o dispatcher de eventos. 
	 **/
	class Event
	{
	public:
		Event() {}

		virtual ~Event() = default;
		virtual EventType GetEventType() const = 0;
		inline virtual std::string ToString() const { return "Event"; }

		const void* NativeEvent;

		bool Handled = false; // Se true, subsistemas posteriores ignoram.
	};
}