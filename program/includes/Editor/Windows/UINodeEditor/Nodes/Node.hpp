#pragma once

#include <imgui/imgui.h>
#include <ImGuiNodeEditor/imgui_node_editor.h>

#include <string>
#include <vector>
#include <variant>
#include <functional>
#include <memory>

namespace editor::nodes
{
	// INE seria a abreviação para imgui node editor, para melhor compreenção, fora do padrão ed.
	namespace ine = ax::NodeEditor;

	// Tipos de valores aceitados em saída/entrada de dados.
	using SocketValue = std::variant<bool, int, float, double, std::string>;

	// ----------------------------------------
	//  Enumeradores, controle de informação
	// ----------------------------------------

	/**
	 * @brief Define os tipos de dados que podem passar pelos sockets de um nó 
	 * 		 (Ponto onde a informação entra e sai) 
	 **/
	enum class SocketType
	{
		Flow,
		
		Int,
		Float,
		Bool,
		String,
		
		Object,
		Function,
		Delegate
	};

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

		KeyboardEvent = 1 << 8, // Indica que é um nó que captura eventos do teclado
		MouseEvent    = 1 << 9, // Indica que é um nó que captura eventos do mouse
		Action        = 1 << 10, // Indica que é um nó de ação, ele tem seu Evaluate chamado no OnUpdate da engine
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

		SocketType Type;
		SocketKind Kind;

		Socket(ine::PinId id, std::string name, SocketType type)
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

		SocketType Type;
		SocketValue Value;
		char InputBuffer[256];

		NodeValue(SocketType type, SocketValue value)
			: SocketPtr(nullptr), Type(type), Value(value)
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
		Socket* AddInput(ine::PinId id, std::string name, SocketType type);

		/**
		 * @brief Adiciona uma saída de dados. (Chamando o callback OnAddOutput)
		 **/
		Socket* AddOutput(ine::PinId id, std::string name, SocketType type);

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
		SocketValue* AddValue(std::string key, SocketType type, SocketValue value);

		/**
		 * @grief Retorna o ponteiro do valor de um node.
		 **/
		SocketValue* GetValue(std::string key);

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