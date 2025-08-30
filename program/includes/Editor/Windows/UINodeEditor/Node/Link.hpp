#pragma once

#include "Editor/Windows/UINodeEditor/Node/Socket.hpp"

#include <imgui/imgui.h>
#include <ImGuiNodeEditor/imgui_node_editor.h>

namespace editor::nodes
{
	// INE seria a abreviação para imgui node editor, para melhor compreenção, fora do padrão ed.
	namespace ine = ax::NodeEditor;

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
}