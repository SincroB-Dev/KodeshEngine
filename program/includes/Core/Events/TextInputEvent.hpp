#pragma once

#include "Core/Events/Event.hpp"

#include <string>

namespace core::events
{
	/**
	 * @brief Evento responsável por capturar entrada de texto padrão, no caso, qualquer
	 *        tipo de texto que não em japones/chines (hiragana, kanjis, etc...) 
	 **/
	class TextInputEvent : public Event
	{
	public:
		TextInputEvent(const char* text): m_Text(text) {}

		const char* GetText() { return m_Text.c_str(); }

		EventType GetEventType() const { return EventType::TextInput; }

	private:
		std::string m_Text;
	};
}