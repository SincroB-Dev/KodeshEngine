#include "Editor/Windows/UINodeEditor/Nodes/InputEventNode.hpp"
#include "Editor/Windows/UINodeEditor.hpp"

#include "Core/Input/KeyState.hpp"

using namespace core::input;

namespace editor::nodes::compositor
{
	void InputEventNode::Mount(UINodeEditor& editor)
	{
		// Usado apenas como meio de entrada para a chamada de seu evaluate.
		AddInput(editor.GetNextId(), "Call", SocketType::Flow);

		AddValue("KeyState", SocketType::Object, KeyStateEnum::Pressed);

		AddOutput(editor.GetNextId(), "Evaluate", SocketType::Flow);
	}

	void InputEventNode::Evaluate()
	{
		KeyStateEnum state = *GetValue("KeyState")->GetObjectPtr<KeyStateEnum>();

		if (m_InputManager.GetState(Button.GetCode()) == state)
		{
			// Pega o socket de saída dos dados
			std::unique_ptr<Socket>& socket = Outputs[0];

			// Executa todos os seus filhos
			for (Socket* child : socket->SocketChilds)
			{
				child->NodePtr->Evaluate();
			}
		}
	}
}