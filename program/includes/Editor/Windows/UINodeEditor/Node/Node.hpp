#pragma once

#include <imgui/imgui.h>
#include <ImGuiNodeEditor/imgui_node_editor.h>

#include "Core/Utils/Metadata.hpp"

#include "Editor/Windows/UINodeEditor/Node/NodeEnums.hpp"
#include "Editor/Windows/UINodeEditor/Node/Socket.hpp"
#include "Editor/Windows/UINodeEditor/Node/NodeValue.hpp"
#include "Editor/Windows/UINodeEditor/Node/Link.hpp"

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
}