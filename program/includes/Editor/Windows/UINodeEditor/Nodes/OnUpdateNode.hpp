#pragma once

#include "Editor/Windows/UINodeEditor/Nodes/Node.hpp"
#include "Editor/Windows/UINodeEditor.hpp"

#include <ImGuiNodeEditor/imgui_node_editor.h>

namespace ine = ax::NodeEditor;

namespace editor::nodes
{
	class UINodeEditor;
}

namespace editor::nodes::compositor
{
	/**
	 * @brief Este node é chamado em todo frame da do game, ele é o principal
	 * 		  responsável por desencadear a corrente de eventos que ocorrem.
	 **/
	class OnUpdateNode : public Node
	{
	public:
		OnUpdateNode(ine::NodeId id): Node(id, "OnUpdate", ImColor(79, 107, 21)/*Verde Musgo*/)
		{
		}

		void Mount(UINodeEditor& editor) override;

		void Evaluate() override;
	};
}