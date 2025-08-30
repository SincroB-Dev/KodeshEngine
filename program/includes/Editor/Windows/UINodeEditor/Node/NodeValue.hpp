#pragma once

#include "Editor/Windows/UINodeEditor/Node/NodeEnums.hpp"
#include "Editor/Windows/UINodeEditor/Node/Socket.hpp"

#include <string>
#include <any>
#include <variant>

namespace editor::nodes
{
	/**
	 * @brief Estrutura que armazena valor e tipo de dados que estrarão em um DataSet de nodes. 
	 **/
	struct NodeValue
	{
		// Socket responsável pela saída/entrada dos dados, caso nullptr, o valor fica armazenado no node.
		Socket* SocketPtr;

		core::MetaType Type;
		SocketValue Value;
		char InputBuffer[256];

		std::string Help;

		/**
		 * @brief Retorna o valor inteiro, garantindo sempre um valor.
		 **/
		inline int GetIntValue()
		{
			if (std::holds_alternative<std::any>(Value))
			{
				return std::get<std::any>(Value).has_value() ? 1 : 0;
			}

		    return std::visit([](auto&& arg) -> int {
		        using T = std::decay_t<decltype(arg)>;
		        if constexpr (std::is_same_v<T, int>) return arg;
		        else if constexpr (std::is_same_v<T, float>) return static_cast<int>(arg);
		        else if constexpr (std::is_same_v<T, bool>) return arg ? 1 : 0;
		        else if constexpr (std::is_same_v<T, std::string>) return -1;
		        else return -1; // fallback
		    }, Value);
		}

		/**
		 * @brief Retorna o valor float, garantindo sempre um valor.
		 **/
		inline float GetFloatValue()
		{
			if (std::holds_alternative<std::any>(Value))
			{
				return std::get<std::any>(Value).has_value() ? 1.0f : 0.0f;
			}

		    return std::visit([](auto&& arg) -> float {
		        using T = std::decay_t<decltype(arg)>;
		        if constexpr (std::is_same_v<T, float>) return arg;
		        else if constexpr (std::is_same_v<T, int>) return static_cast<float>(arg);
		        else if constexpr (std::is_same_v<T, bool>) return arg ? 1.0f : 0.0f;
		        else if constexpr (std::is_same_v<T, std::string>) return -1.0f;
		        else return -1.0f;
		    }, Value);
		}

		/**
		 * @brief Retorna o valor boleano, garantindo sempre um valor.
		 **/
		inline bool GetBoolValue()
		{
			if (std::holds_alternative<std::any>(Value))
			{
				return std::get<std::any>(Value).has_value();
			}

		    return std::visit([](auto&& arg) -> bool {
		        using T = std::decay_t<decltype(arg)>;
		        if constexpr (std::is_same_v<T, bool>) return arg;
		        else if constexpr (std::is_same_v<T, int>) return arg != 0;
		        else if constexpr (std::is_same_v<T, float>) return arg != 0.0f;
		        else if constexpr (std::is_same_v<T, std::string>) return !arg.empty();
		        else return false;
		    }, Value);
		}

		/**
		 * @brief Retorna uma string, garantindo sempre um valor.
		 **/
		inline std::string GetStringValue()
		{
			if (std::holds_alternative<std::any>(Value))
			{
				return "<Object>";
			}

		    return std::visit([](auto&& arg) -> std::string {
		        using T = std::decay_t<decltype(arg)>;
		        if constexpr (std::is_same_v<T, std::string>) return arg;
		        else if constexpr (std::is_same_v<T, int>) return std::to_string(arg);
		        else if constexpr (std::is_same_v<T, float>) return std::to_string(arg);
		        else if constexpr (std::is_same_v<T, bool>) return arg ? "true" : "false";
		        else return "";
		    }, Value);
		}

		/**
		 * @brief Retorna um objeto encapsulado em std::any. 
		 **/
		template<typename T>
		inline T* GetObjectPtr()
		{
			if (std::holds_alternative<std::any>(Value))
			{
				std::any& v = std::get<std::any>(Value);

				if (v.has_value())
				{
					return std::any_cast<T>(&v);
				}
			}

			return nullptr;
		}

		NodeValue(core::MetaType type, SocketValue value, std::string help = "")
			: SocketPtr(nullptr), Type(type), Value(value), Help(help)
		{}
	};
}