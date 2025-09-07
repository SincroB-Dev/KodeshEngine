#pragma once

#include <string>
#include "Core/Events/EventDispatcher.hpp"

namespace core::serialization
{
	/**
	 * @brief Responsável por salvar arquivos, facilitando o encapsulamento dos dados 
	 *        e trabalhos migratórios no futuro. 
	 **/
	class SaveSystem
	{
	public:
		SaveSystem(events::EventDispatcher& dispatcher): ke_Dispatcher(dispatcher) {}

		/**
		 * @brief Metodo responsável por salvar dados em um arquivo json. 
		 **/
		void Save(const std::string& path);

		/**
		 * @brief Metodo responsável por carregar dados de um arquivo json. 
		 **/
		void Load(const std::string& path);

	private:
		events::EventDispatcher& ke_Dispatcher;
	};
}