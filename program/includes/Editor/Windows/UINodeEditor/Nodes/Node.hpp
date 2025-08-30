#pragma once

#include <imgui/imgui.h>
#include <ImGuiNodeEditor/imgui_node_editor.h>

#include "Core/Utils/Metadata.hpp"

#include <string>
#include <vector>
#include <variant>
#include <functional>
#include <memory>
#include <any>

namespace editor::nodes
{
	// INE seria a abreviação para imgui node editor, para melhor compreenção, fora do padrão ed.
	namespace ine = ax::NodeEditor;

	// Tipos de valores aceitados em saída/entrada de dados.
	using SocketValue = std::variant<bool, int, float, double, std::string, std::any>;

	// ----------------------------------------
	//  Enumeradores, controle de informação
	// ----------------------------------------

	/**
	 * @brief Especifica a natureza do socket, entrada ou saída de informações.
	 **/
	enum class SocketKind
	{
		Input,
		Output
	};

	/**
	 * @brief Indica o tipo de nó dentro do editor, cada nó tem uma finalidade diferente.
	 **/
	enum class NodeType: uint32_t
	{
		None      = 0,
		Blueprint = 1 << 0, // Nó lógico principal
		Simple    = 1 << 1, // Nó basico para operações diretas
		Tree      = 1 << 2, // Nó hierarquico (Para decisões em arvore)
		Comment   = 1 << 3, // Para organização visual
		Houdini   = 1 << 4, // Nó especial de integração externa

		InputEvent = 1 << 8, // Nó que trabalha com entrada de inputs de usuário (teclado, mouse, gamepad)
		Action     = 1 << 10, // Indica que é um nó de ação, ele tem seu Evaluate chamado no OnUpdate da engine
	};

	/**
	 * @brief Bitwise OR inclusivo.
	 **/
	inline NodeType operator | (NodeType a, NodeType b)
	{
		return static_cast<NodeType>(
			static_cast<uint32_t>(a) | static_cast<uint32_t>(b)
		);
	} 

	/**
	 * @brief Bitwise AND comparativo
	 **/
	inline NodeType operator & (NodeType a, NodeType b)
	{
		return static_cast<NodeType>(
			static_cast<uint32_t>(a) & static_cast<uint32_t>(b)
		);
	}

	// --------------------------------------------
	//  Estruturas, armazenamento de informações
	// --------------------------------------------

	struct Node; // Pré definição para uso em Socket

	/**
	 * @brief Representa o ponto de conexão de um nó (Node) 
	 **/
	struct Socket
	{
		ine::PinId  ID;
		std::string Name;

		Node* NodePtr;
		std::vector<Socket*> SocketChilds;

		core::MetaType Type;
		SocketKind Kind;

		Socket(ine::PinId id, std::string name, core::MetaType type)
			: ID(id), Name(name), NodePtr(nullptr), Type(type), Kind(SocketKind::Input)
		{}
	};

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

	// Abstração para uso posterior.
	class UINodeEditor;

	/**
	 * @brief Representa um nó (Node) completo. 
	 **/
	class Node
	{
	public:
		ine::NodeId ID;
		std::string Name;

		std::vector<std::unique_ptr<Socket>> Inputs;
		std::vector<std::unique_ptr<Socket>> Outputs;

		/**
		 * @brief Adiciona uma entrada de dados. (Chamando o callback OnAddInput)
		 **/
		Socket* AddInput(ine::PinId id, std::string name, core::MetaType type);

		/**
		 * @brief Adiciona uma saída de dados. (Chamando o callback OnAddOutput)
		 **/
		Socket* AddOutput(ine::PinId id, std::string name, core::MetaType type);

		/**
		 * @brief Remove uma entrada de dados. (Chamando o callback OnRemoveInput)
		 **/
		void RemoveInput(ine::PinId);

		/**
		 * @brief Remove uma saída de dados. (Chamando o callback OnRemoveOutput)
		 **/
		void RemoveOutput(ine::PinId);

		/**
		 * @brief Adiciona uma propriedade de valor ao node.
		 **/
		SocketValue* AddValue(std::string key, core::MetaType type, SocketValue value, std::string help = "");

		/**
		 * @grief Retorna o ponteiro do valor de um node.
		 **/
		NodeValue* GetValue(std::string key);

		/**
		 * @brief Conecta uma propriedade de valor a um socket de entrada do node.
		 * 		  O socket para ser conectado, deve ser filho do node, caso contrário,
		 * 		  a conexão não é feita e é reportado um problema.
		 * @return true caso o link tenha sido feito, false caso contrário.
		 **/
		bool LinkValueToOutput(std::string key, Socket* socket);

		/**
		 * @brief Conecta uma propriedade de valor a um socket de saída do node.
		 * 		  O socket para ser conectado, deve ser filho do node, caso contrário,
		 * 		  a conexão não é feita e é reportado um problema.
		 * @return true caso o link tenha sido feito, false caso contrário.
		 **/
		bool LinkValueToInput(std::string key, Socket* socket);

		//-----------------------------
		// Callbacks
		//-----------------------------
		std::function<void(ine::PinId, Socket* socket)> OnAddInput;
		std::function<void(ine::PinId, Socket* socket)> OnAddOutput;

		std::function<void(ine::PinId)> OnRemoveInput;
		std::function<void(ine::PinId)> OnRemoveOutput;

		//----------------------------
		// Setups visuais
		//----------------------------
		ImColor  Color;
		ImVec2   Size;

		//----------------------------
		// Estados do nó
		//----------------------------
		std::string State;
		std::string SavedState;

		//----------------------------
		// Dados
		//----------------------------
		std::string Comment;
		std::unordered_map<std::string, std::unique_ptr<NodeValue>> DataSet;

		Node(ine::NodeId id, std::string name, ImColor Color = ImColor(255, 255, 255))
			: ID(id), Name(name), Size(0, 0)
		{}
		~Node() = default;

		/**
		 * @brief Função responsável pela montagem do node.
		 **/
		virtual void Mount(UINodeEditor& editor) {}

		/**
		 * @brief Função responsável pela ativação de filhos do pin flow.
		 **/
		virtual void Evaluate() {}

		/**
		 * @brief Retorna a tipagem do node.
		 **/
		virtual NodeType GetType() const { return NodeType::Blueprint; }

		/**
		 * @brief Verifica se o node possuí um tipo específico.
		 **/
		inline bool IsAnyType(NodeType type)
		{
			return (static_cast<uint32_t>(GetType()) & static_cast<uint32_t>(type)) != 0;
		}

	private:

		/**
		 * @brief Faz a validação do tipo que será inserido.
		 **/
		bool ValidateType(core::MetaType type, const SocketValue& v);

	private:
		static unsigned int s_NextUIID;
	};

	/**
	 * @brief Representa uma conexão entre dois sockets 
	 **/
	class Link
	{
	public:
		ine::LinkId ID;

		Socket* Output;
		Socket* Input;

		ImColor Color;

		Link(ine::LinkId id, Socket* start, Socket* end)
			: ID(id), Output(start), Input(end)
		{}
		~Link() = default;
	};

	// -------------------------------------------------------
	//  Helpers : Functor de comparação, e função utilitaria
	// -------------------------------------------------------
	
	/**
	 * @brief Node de comparação, usado para ordenar /ou armazenar nós em containers
	 *        que exigem comparação, garantindo uma forma consistente de comparar nós
	 * 		  pelo ponteiro interno, e evitando ambiguidades em buscas e inserções.
	 **/
	struct NodeIdLess
	{
		bool operator()(const ine::NodeId& lhs, const ine::NodeId& rhs) const
		{
			return lhs.AsPointer() < rhs.AsPointer();
		}
	};


}