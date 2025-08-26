#pragma once

#include "Core/Events/Event.hpp"

#include <functional>
#include <unordered_map>
#include <vector>
#include <typeindex>
#include <memory>

namespace core::events
{
	/**
	 * @brief Centraliza todos os eventos em um vector, e os chama quando um evento
	 *        for acionado pela interface responsável pela janela. 
	 **/
	class EventDispatcher
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		// Registra callback para um tipo especifico de evento
		template<typename EventType>
		void Register(const EventCallbackFn& callback);

		// Dispara o evento para todos os callbacks registrados
		inline void Dispatch(Event& event)
		{
			auto it = m_Callbacks.find(typeid(event));
			if (it != m_Callbacks.end())
			{
				for (auto& callback : it->second)
				{
					callback(event);
					// Com isso dá para marcar event.Handled = true quando clicar em um botão
					// evitando passar para scene.
					if (event.Handled) break; // Interrompe se consumido;
				}
			}
		}

	private:
		// Armazena uma lista de callbacks para cada tipo de evento.
		std::unordered_map<std::type_index, std::vector<EventCallbackFn>> m_Callbacks;
	};

	// Registra callback para um tipo especifico de evento
	template<typename EventType>
	inline void EventDispatcher::Register(const EventCallbackFn& callback)
	{
		auto& vec = m_Callbacks[typeid(EventType)];
		vec.push_back(callback);
	}
}