#pragma once

#include "Editor/Windows/UIWindow.hpp"
#include "Editor/Windows/UINodeEditor/Widgets.hpp"

#include <ImGuiNodeEditor/imgui_node_editor.h>

#include "Editor/Windows/UINodeEditor/Nodes/Node.hpp"
#include "Core/Renderer/Renderer.hpp"

#include <imgui/imgui.h>
#include <vector>
#include <map>
#include <unordered_map>

namespace editor::nodes
{
	// Abstração para friedship.
    namespace compositor
    {
        class Compositor;
    }

	namespace ine = ax::NodeEditor;

	class UINodeEditor : public windows::UIWindow
	{
	public:
		UINodeEditor(core::renderer::Renderer& renderer);
		~UINodeEditor();

		/**
		 * @brief Pega o próximo id disponível.
		 **/
		int GetNextId();

		/**
		 * @brief Pega a cor correspondente ao socket.
		 **/
		ImColor GetIconColor(SocketType socketType);

		/**
		 * @brief Desenha o icone correspondente ao socket, também o preenche quando conectado.
		 **/
		void DrawPinIcon(const Socket& socket, bool connected, int alpha);

		/**
		 * @brief Procura um node no grafo.
		 **/
		Node* FindNode(ine::NodeId id);

		/**
		 * @brief Procura um socket no grafo.
		 **/
		Socket* FindSocket(ine::PinId id);

		/**
		 * @brief Procura um link no grafo.
		 **/
		Link* FindLink(ine::LinkId id);

		/**
		 * @brief Verificador de conexão de socket.
		 */
		bool IsSocketLinked(ine::PinId id) const;

		/**
		 * @brief Verifica se dois sockets podem ser conectados, garantindo que
		 *        a conexão siga as regras da engine de nodes.
		 *
		 * Condições para a criação do link:
		 *  - Nenhum dos sockets pode ser nulo.
		 *  - Os sockets não podem ser o mesmo objeto.
		 *  - Os sockets devem ter tipos de dado iguais (Type).
		 *  - Os sockets devem ter Kind diferentes (input/output).
		 *  - Os sockets não podem pertencer ao mesmo node (NodePtr).
		 *
		 * Além disso, a função ajusta os ponteiros de forma que o socket de saída
		 * (Output) seja sempre o início da conexão e o socket de entrada (Input)
		 * seja o fim. Se o socket de saída recebido for na verdade um Input, os
		 * ponteiros são trocados para manter essa convenção, facilitando a leitura
		 * e manipulação dos links posteriormente.
		 */
		bool CanCreateLink(Socket*& a, Socket*& b);

		/**
		 * @brief Responsável por detectar e aplicar a criação de um novo link entre nós.
		 **/
		void HandleLinkCreate();
		/**
		 * @brief Responsável por excluir um nó do grafo.
		 **/
		void HandleLinkDelete();

		/**
		 * @brief Remove um socket do lookup. (optimização de buscas de sockets)
		 **/
		void RemoveSocketFromLookup(ine::PinId id);

		/**
		 * @brief Adiciona um socket no lookup. (optimização de buscas de sockets)
		 **/
		void AddSocketToLookup(ine::PinId id, Socket* socket);

		// --------------------------------------
		// Métodos de renderização
		// --------------------------------------

		/**
		 * @brief Desenha os campos editaveis do node. 
		 **/
		void RenderValue(const char* label, NodeValue* value);

		/**
		 * @brief Desenha um nó de blueprint (GameLogic). 
		 **/
		void RenderBlueprintNode(Node* node);

		void OnImGuiRender() override;

	private:
	    unsigned int         m_HeaderBackground;
		unsigned int         m_SaveIcon;
		unsigned int         m_RestoreIcon;

	    int                  m_NextId;
	    const int            m_SocketIconSize;

	    std::vector<Node>    m_Nodes;
	    std::vector<Link>    m_Links;

	    std::unordered_map<int, Socket*> m_SocketLookup;

	    ine::EditorContext* m_Context;

	    core::renderer::Renderer& m_Renderer;

	    // Recebeu friend para dar entradas no m_SocketLookup
	    friend class compositor::Compositor;
	};
}