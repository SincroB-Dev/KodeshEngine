#pragma once

#include "Core/Events/Event.hpp"

namespace core::events
{
	/**
	 * @brief Estados da engine, estes estados vão definir quando sistemas poderão ser executados e quando cargas pesadas estarão em funcionamento. 
	 **/
	enum class KodeshModeEnum: uint32_t
	{
		NONE      = 0,		// Modo neutro para comparativos.
		EDIT_MODE = 1 << 0, // Modo em que a engine não executa sistemas de cenas, ela é propria para edição.
		PLAY_MODE = 1 << 1, // Modo em que o jogo está em execução, ao entrar nesse modo é feito um backup do gerenciador de cenas.

		SWAPPING  = 1 << 2 // Swapping é reservado da engine, ele é utilizado para recarregar sistemas sem afetar sistemas funcionais, tudo é carregado em swapping, e depois'é lançado para o modo atual da engine.
	};

	/**
	 * @brief Bitwise OR inclusivo.
	 **/
	inline KodeshModeEnum operator | (KodeshModeEnum a, KodeshModeEnum b)
	{
		return static_cast<KodeshModeEnum>(
			static_cast<uint32_t>(a) | static_cast<uint32_t>(b)
		);
	} 

	/**
	 * @brief Bitwise AND comparativo
	 **/
	inline KodeshModeEnum operator & (KodeshModeEnum a, KodeshModeEnum b)
	{
		return static_cast<KodeshModeEnum>(
			static_cast<uint32_t>(a) & static_cast<uint32_t>(b)
		);
	}

	/**
	 * @brief Evento que sinaliza quando a engine troca de modo.
	 **/
	class KodeshModeChangedEvent : public Event
	{
	public:
		KodeshModeChangedEvent(KodeshModeEnum oldMode, KodeshModeEnum newMode)
			: m_OldMode(oldMode), m_NewMode(newMode) {}

		inline KodeshModeEnum GetOldMode() const { return m_OldMode; }
		inline KodeshModeEnum GetNewMode() const { return m_NewMode; }

		EventType GetEventType() const override { return EventType::KodeshModeChanged; }

	private:
		KodeshModeEnum m_OldMode;
		KodeshModeEnum m_NewMode;
	};
}