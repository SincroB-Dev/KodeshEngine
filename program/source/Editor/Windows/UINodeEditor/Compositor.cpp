#include "Editor/Windows/UINodeEditor/Compositor.hpp"
#include "Editor/Windows/UINodeEditor/Interfaces/InputEventNode.hpp"
#include "Editor/Windows/UINodeEditor/Interfaces/OnUpdateNode.hpp"
#include <ImGuiNodeEditor/imgui_node_editor.h>
#include <memory>

namespace ine = ax::NodeEditor;

namespace editor::nodes::compositor
{
	void Compositor::AddSocketsCallbacks(UINodeEditor* editor, Node* node)
    {
        // Adiciona callbacks para cada tipo de ação, permitindo o nó saber oque fazer quando entrar no editor.
        node->OnAddInput = [editor](ine::PinId id, Socket* socket){ 
            editor->AddSocketToLookup(id, socket);
        };
        node->OnAddOutput = [editor](ine::PinId id, Socket* socket){ 
            editor->AddSocketToLookup(id, socket);
        };
        node->OnRemoveInput = [editor](ine::PinId id){ 
            editor->RemoveSocketFromLookup(id);
        };
        node->OnRemoveOutput = [editor](ine::PinId id){ 
            editor->RemoveSocketFromLookup(id);
        };
    }

	//-----------------------------
	// Inputs básicos
	//-----------------------------

	Node* Compositor::InputEventNode(core::app::KodeshApplication& app, UINodeEditor* editor)
	{
		auto& refNodes = editor->m_Nodes;

		auto node = std::make_unique<compositor::InputEventNode>(editor->GetNextId(), app.GetInputManager());
		Node* ptr = node.get();

        refNodes.emplace_back(std::move(node));

        // Adiciona callbacks para cada tipo de ação, permitindo o nó saber oque fazer quando entrar no editor.
        AddSocketsCallbacks(editor, ptr);

        ptr->Mount(*editor);
    
        return ptr;
	}

	Node* Compositor::OnUpdateNode(core::app::KodeshApplication& app, UINodeEditor* editor)
	{
		auto& refNodes = editor->m_Nodes;

		auto node = std::make_unique<compositor::OnUpdateNode>(editor->GetNextId());
		Node* ptr = node.get();

        refNodes.emplace_back(std::move(node));

        // Adiciona callbacks para cada tipo de ação, permitindo o nó saber oque fazer quando entrar no editor.
        AddSocketsCallbacks(editor, ptr);

        ptr->Mount(*editor);
    
        return ptr;
	}
}