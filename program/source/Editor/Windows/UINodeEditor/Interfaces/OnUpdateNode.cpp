#include "Editor/Windows/UINodeEditor/Interfaces/OnUpdateNode.hpp"
#include "Editor/Windows/UINodeEditor.hpp"
#include "Core/Utils/Metadata.hpp"

namespace editor::nodes::compositor
{
	void OnUpdateNode::Mount(UINodeEditor& editor)
	{
		AddOutput(editor.GetNextId(), "OnEvent", core::MetaType::Flow);
	}

	void OnUpdateNode::Evaluate()
	{
		if (Outputs.size() == 1)
		{
			std::unique_ptr<Socket>& socket = Outputs[0];

			for (Socket* child : socket->SocketChilds)
			{
				child->NodePtr->Evaluate();
			}
		}
	}
}