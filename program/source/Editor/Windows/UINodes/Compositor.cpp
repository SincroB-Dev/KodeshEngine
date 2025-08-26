#include "Editor/Windows/UINodes/Compositor.hpp"
#include <ImGuiNodeEditor/imgui_node_editor.h>

namespace ine = ax::NodeEditor;

namespace editor::nodes::compositor
{
	Node* Compositor::BaseNode(UINodeEditor* editor, std::string name)
    {
    	auto& refNodes = editor->m_Nodes;

    	// Adiciona o node básico.
        refNodes.emplace_back(editor->GetNextId(), name);

        // Adiciona callbacks para cada tipo de ação, permitindo o nó saber oque fazer quando entrar no editor.
        refNodes.back().OnAddInput = [editor](ine::PinId id, Socket* socket){ 
            editor->AddSocketToLookup(id, socket);
        };
        refNodes.back().OnAddOutput = [editor](ine::PinId id, Socket* socket){ 
            editor->AddSocketToLookup(id, socket);
        };
        refNodes.back().OnRemoveInput = [editor](ine::PinId id){ 
            editor->RemoveSocketFromLookup(id);
        };
        refNodes.back().OnRemoveOutput = [editor](ine::PinId id){ 
            editor->RemoveSocketFromLookup(id);
        };
    
        return &refNodes.back();
    }

	//-----------------------------
	// Inputs básicos
	//-----------------------------

	Node* Compositor::KeyboardEventNode(UINodeEditor* editor)
	{
		Node* node = BaseNode(editor, "KeyboardEventNode");

		// Inputs/Outputs
		node->AddOutput(editor->GetNextId(), "KeyStatusOut", SocketType::Flow);

		// Values
		node->AddValue("Key", SocketType::String, std::string(""));

        return node;
	}

	Node* Compositor::MouseEventNode(UINodeEditor* editor)
	{
		Node* node = BaseNode(editor, "MouseEventNode");

		// Inputs/Outputs
		node->AddOutput(editor->GetNextId(), "MouseStatusOut", SocketType::Flow);
		node->AddInput(editor->GetNextId(), "Test", SocketType::Bool);

		// Values
		node->AddValue("Button", SocketType::String, std::string(""));

        return node;
	}

	Node* Compositor::AlwaysEventNode(UINodeEditor* editor)
	{
		Node* node = BaseNode(editor, "AlwaysEventNode");

		// Inputs/Outputs
		node->AddOutput(editor->GetNextId(), "MouseStatusOut", SocketType::Flow);

		// Values
		//-----

        return node;
	}

	//-------------------------------
	// Captura da cena
	//-------------------------------

	Node* Compositor::GetEntityNode(UINodeEditor* editor)
	{
		Node* node = BaseNode(editor, "GetEntityNode");

		// Inputs/Outputs
		node->AddOutput(editor->GetNextId(), "MouseStatusOut", SocketType::Flow);

		// Values
		node->AddValue("Name", SocketType::String, std::string(""));

        return node;
	}

	Node* Compositor::TestOutputNode(UINodeEditor* editor)
	{
		Node* node = BaseNode(editor, "TestOutputNode");

		// Inputs/Outputs
		node->AddInput(editor->GetNextId(), "Entradas", SocketType::Flow);

        return node;
	}
	
}