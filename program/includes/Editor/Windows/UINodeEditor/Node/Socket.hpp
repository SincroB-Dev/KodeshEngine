#pragma once

#include <string>
#include <variant>
#include <any>
#include <vector>

#include "Core/Utils/Metadata.hpp"

#include <ImGuiNodeEditor/imgui_node_editor.h>

namespace editor::nodes
{
	namespace ine = ax::NodeEditor;

	// Tipos de valores aceitados em saída/entrada de dados.
	using SocketValue = std::variant<bool, int, float, double, std::string, std::any>;

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
}