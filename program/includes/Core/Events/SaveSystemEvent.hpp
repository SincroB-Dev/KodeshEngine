#pragma once

#include "Core/Events/Event.hpp"
#include <nlohmann/json.hpp>

namespace core::events
{
	/**
	 * @brief Evento responsável por agrupar dados para serem salvos em um arquivo.
	 **/
	class SaveProjectEvent : public Event
	{
	public:
		SaveProjectEvent(nlohmann::json& data)
			: m_Data(data) {}

		// Consulta de dados
		nlohmann::json GetData() const { return m_Data; }

		EventType GetEventType() const override { return EventType::SaveProject; }

	    // operador[] para chave string → retorna referência para json interno
	    nlohmann::json& operator[](const std::string& key)
	    {
	        return m_Data[key];
	    }

	    // operador[] para índice numérico → retorna referência para json interno
	    nlohmann::json& operator[](size_t index)
	    {
	        return m_Data[index];
	    }

	    // Const version
	    const nlohmann::json& operator[](const std::string& key) const
	    {
	        return m_Data.at(key);
	    }

	    const nlohmann::json& operator[](size_t index) const
	    {
	        return m_Data.at(index);
	    }

	private:
		nlohmann::json& m_Data;
	};

	/**
	 * @brief Evento responsável por agrupar dados para serem salvos em um arquivo.
	 **/
	class LoadProjectEvent : public Event
	{
	public:
		LoadProjectEvent(nlohmann::json& data)
			: m_Data(data) {}

		EventType GetEventType() const override { return EventType::LoadProject; }

		// Consulta de dados
		nlohmann::json GetData() const { return m_Data; }

	    // operador[] para chave string → retorna referência para json interno
	    nlohmann::json& operator[](const std::string& key)
	    {
	        return m_Data[key];
	    }

	    // operador[] para índice numérico → retorna referência para json interno
	    nlohmann::json& operator[](size_t index)
	    {
	        return m_Data[index];
	    }

	    // Const version
	    const nlohmann::json& operator[](const std::string& key) const
	    {
	        return m_Data.at(key);
	    }

	    const nlohmann::json& operator[](size_t index) const
	    {
	        return m_Data.at(index);
	    }

	private:
		nlohmann::json& m_Data;
	};
}