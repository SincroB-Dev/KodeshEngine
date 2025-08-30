#pragma once

#include "Editor/Windows/UINodeEditor/Nodes/Node.hpp"
#include "Editor/UI/UIPressButton.hpp"
#include "Core/Input/InputManager.hpp"

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
	class InputEventNode : public Node
	{
	public:
		ui::UIPressButton Button;

		InputEventNode(ine::NodeId id, core::input::InputManager& inputs)
			: Node(id, "InputEventNode", ImColor(21, 89, 107)/*Azul Levemente Escuro*/),
			Button(ui::UIPressButtonType::Keyboard, "<Empty>", "<Press Key>"),
			m_InputManager(inputs)
		{}

		void Mount(UINodeEditor& editor) override;

		void Evaluate() override;

		NodeType GetType() const override { return NodeType::Blueprint | NodeType::InputEvent; }

	private:
		core::input::InputManager& m_InputManager;
	};
}