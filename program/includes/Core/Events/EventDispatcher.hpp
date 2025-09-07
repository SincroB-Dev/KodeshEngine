#pragma once

#include "Core/Events/Event.hpp"
#include "Core/Utils/UniqueIDGen.hpp"

#include <functional>
#include <unordered_map>
#include <vector>
#include <typeindex>
#include <memory>

namespace core::events
{
	/**
	 * @brief Callback de eventos. 
	 **/
	using EventCallbackFn = std::function<void(Event&)>;

	/**
	 * @brief Entrada de eventos, mais fácil para fazer filtragem. 
	 **/
	struct EventEntry
	{
		void* Owner;
		EventCallbackFn Callback;
		utils::UniqueID UID;
	};

	/**
	 * @brief Centraliza todos os eventos em um vector, e os chama quando um evento
	 *        for acionado pela interface responsável pela janela. 
	 **/
	class EventDispatcher
	{
	public:
		// Registra callback para um tipo especifico de evento
		template<typename EventType>
		void Register(void* owner, const EventCallbackFn& callback);

		// Remoção de callback utilizando o uniqueid
		void Unregister(utils::UniqueID id);

		// Remoção de callback utilizando o proprietário (remove TODOS os callbacks com um determinado proprietário)
		void Unregister(void* owner);

		// Dispara o evento para todos os callbacks registrados
		void Dispatch(Event& event);

		// Gerador de identificação de callbacks do sistema.
		static utils::UniqueIDGen s_NextID;

	private:
		// Armazena uma lista de callbacks para cada tipo de evento.
		std::unordered_map<std::type_index, std::vector<EventEntry>> m_Callbacks;
	};

	// Registra callback para um tipo especifico de evento
	template<typename EventType>
	inline void EventDispatcher::Register(void* owner, const EventCallbackFn& callback)
	{
		auto& vec = m_Callbacks[typeid(EventType)];
		vec.push_back(EventEntry{
			owner,
			callback,
			s_NextID.CreateUniqueID()
		});
	}
}