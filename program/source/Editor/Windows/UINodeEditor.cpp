#include <ImGuiNodeEditor/imgui_node_editor.h>

#include "Core/Systems/LogManager.hpp"
#include "Editor/Windows/UINodeEditor.hpp"

#include <iostream>
#include <algorithm>
#include <variant>
#include <string>

using namespace core::systems;
using namespace core::renderer;

namespace editor::nodes
{
	using namespace drawing;

	UINodeEditor::UINodeEditor(Renderer& renderer)
		: windows::UIWindow("[Compositor de Node Grafo : GameLogic]"),
          m_NextId(1), m_SocketIconSize(24), m_Renderer(renderer)
	{
		ine::Config config;
		
        config.SettingsFile = "sampleNodes.json";
        config.UserPointer = this;

		m_Context = ine::CreateEditor(&config);

        m_HeaderBackground = m_Renderer.LoadTexture("assets/images/BlueprintBackground.png");
        m_SaveIcon         = m_Renderer.LoadTexture("assets/images/ic_save_white_24dp.png");
        m_RestoreIcon      = m_Renderer.LoadTexture("assets/images/ic_restore_white_24dp.png");
	}

	UINodeEditor::~UINodeEditor()
	{
		if (m_Context)
	    {
            std::cout << "[NodeEditor] Limpando context..." << std::endl;

	        ine::DestroyEditor(m_Context);
	        m_Context = nullptr;
	    }
	}

    int UINodeEditor::GetNextId()
    {
        return m_NextId++;
    }

    //------------------------------
    // Estética
    //------------------------------
	ImColor UINodeEditor::GetIconColor(SocketType socketType)
    {
        switch (socketType)
        {
            default:
            case SocketType::Flow:     return ImColor(255, 255, 255);
            case SocketType::Bool:     return ImColor(220,  48,  48);
            case SocketType::Int:      return ImColor( 68, 201, 156);
            case SocketType::Float:    return ImColor(147, 226,  74);
            case SocketType::String:   return ImColor(124,  21, 153);
            case SocketType::Entity:   return ImColor( 51, 150, 215);
            case SocketType::Function: return ImColor(218,   0, 183);
            case SocketType::Delegate: return ImColor(255,  48,  48);
        }
    };

    void UINodeEditor::DrawPinIcon(const Socket& socket, bool connected, int alpha)
    {
        IconType iconType;
        ImColor  color = GetIconColor(socket.Type);
        color.Value.w = alpha / 255.0f;
        switch (socket.Type)
        {
            case SocketType::Flow:     iconType = IconType::Flow;   break;
            case SocketType::Bool:     iconType = IconType::Circle; break;
            case SocketType::Int:      iconType = IconType::Circle; break;
            case SocketType::Float:    iconType = IconType::Circle; break;
            case SocketType::String:   iconType = IconType::Circle; break;
            case SocketType::Entity:   iconType = IconType::Circle; break;
            case SocketType::Function: iconType = IconType::Circle; break;
            case SocketType::Delegate: iconType = IconType::Square; break;
            default:
                return;
        }

        widgets::SocketIcon(
        	ImVec2(
        		static_cast<float>(m_SocketIconSize), 
        		static_cast<float>(m_SocketIconSize)), 
    		iconType, 
    		connected, 
    		color, 
    		ImColor(32, 32, 32, alpha)
		);
    }

    //-----------------------------------
    // Pesquisas
    //-----------------------------------
    Node* UINodeEditor::FindNode(ine::NodeId id)
    {
        for (auto& node : m_Nodes)
            if (node.ID == id)
                return &node;

        return nullptr;
    }

    Link* UINodeEditor::FindLink(ine::LinkId id)
    {
        for (auto& link : m_Links)
            if (link.ID == id)
                return &link;

        return nullptr;
    }

    Socket* UINodeEditor::FindSocket(ine::PinId id)
    {
        int id_ = static_cast<int>(id.Get());

        auto it = m_SocketLookup.find(id_);
        if (it != m_SocketLookup.end())
        {
            return it->second;
        }
        return nullptr;
    }

    //--------------------------------------
    // Integração com Lookup
    //--------------------------------------
    void UINodeEditor::RemoveSocketFromLookup(ine::PinId id)
    {
        int id_ = static_cast<int>(id.Get());

        auto pin = m_SocketLookup.find(id_);
        if (pin != m_SocketLookup.end())
        {
            m_SocketLookup.erase(pin);
        }
    }

    void UINodeEditor::AddSocketToLookup(ine::PinId id, Socket* socket)
    {
        int id_ = static_cast<int>(id.Get());

        auto pin = m_SocketLookup.find(id_);
        if (pin == m_SocketLookup.end())
        {
            m_SocketLookup.emplace(id_, socket);
        }
    }

    //-----------------------------------
    // Testes de link
    //-----------------------------------
    bool UINodeEditor::IsSocketLinked(ine::PinId id) const
    {
        if (!id)
            return false;

        for (auto& link : m_Links)
            if (link.StartSocketId == id || link.EndSocketId == id)
                return true;

        return false;
    }

    bool UINodeEditor::CanCreateLink(Socket* a, Socket* b) const
    {
        return !(
        	!a || !b || a == b || 
    		 a->Kind == b->Kind ||
    		 a->Type != b->Type || 
    		 a->NodePtr == b->NodePtr
		);
    }

    //---------------------------------
    // Construtor de nodes
    //---------------------------------
    Node* UINodeEditor::CreateBaseNode(std::string name)
    {
        m_Nodes.emplace_back(GetNextId(), name);

        m_Nodes.back().OnAddInput = [this](ine::PinId id, Socket* socket){ 
            this->AddSocketToLookup(id, socket);
        };
        m_Nodes.back().OnAddOutput = [this](ine::PinId id, Socket* socket){ 
            this->AddSocketToLookup(id, socket);
        };
        m_Nodes.back().OnRemoveInput = [this](ine::PinId id){ 
            this->RemoveSocketFromLookup(id);
        };
        m_Nodes.back().OnRemoveOutput = [this](ine::PinId id){ 
            this->RemoveSocketFromLookup(id);
        };
    
        return &m_Nodes.back();
    }

    Node* UINodeEditor::SpawnOutputActionNode()
    {
        Node* node = CreateBaseNode("Output");

        node->AddValue("float", SocketType::Float, 10.0f);
        node->AddValue("int", SocketType::Int, 5);
        node->AddValue("string", SocketType::String, std::string("W"));
        node->AddValue("bool", SocketType::Bool, false);

        node->AddInput(GetNextId(), "Key", SocketType::String);
        node->AddInput(GetNextId(), "Velocity", SocketType::Float);

        node->AddOutput(GetNextId(), "VelocityBasedOnKey", SocketType::Float);

        return node;
    }

    void UINodeEditor::HandleLinkCreate()
    {
        if (ine::BeginCreate())
        {
            ine::PinId inputPinId, outputPinId;

            if (ine::QueryNewLink(&inputPinId, &outputPinId, ImVec4(1.0f, 0.0f, 0.0f, 1.0f)))
            {
                if (inputPinId && outputPinId)
                {
                    if (ine::AcceptNewItem())
                    {
                        Socket* start = FindSocket(inputPinId);
                        Socket* end = FindSocket(outputPinId);

                        if (CanCreateLink(start, end))
                        {
                            m_Links.push_back(Link(GetNextId(), inputPinId, outputPinId));
                        }
                    }
                }
            }
        }
        ine::EndCreate();
    }

    void UINodeEditor::HandleLinkDelete()
    {
        if (ine::BeginDelete())
        {
            ine::LinkId deletedLinkId;
            while (ine::QueryDeletedLink(&deletedLinkId))
            {
                if (ine::AcceptDeletedItem())
                {
                    m_Links.erase(
                        std::remove_if(
                            m_Links.begin(), 
                            m_Links.end(), 
                            [deletedLinkId](const Link& link) {
                                return link.ID == deletedLinkId;
                            }
                        ), m_Links.end()
                    );
                }
            }
        }
        ine::EndDelete();
    }

    //----------------------------------
    // Renderização
    //----------------------------------
    void UINodeEditor::RenderValue(const char* label, NodeValue* objVal)
    {
        std::visit([&](auto& v){
            using T = std::decay_t<decltype(v)>;

            if constexpr (std::is_same_v<T, int>)
            {
                ImGui::InputInt(label, &v);
            }
            else if constexpr (std::is_same_v<T, float>)
            {
                ImGui::InputFloat(label, &v);
            }
            else if constexpr (std::is_same_v<T, bool>)
            {
                ImGui::Checkbox(label, &v);
            }

            else if constexpr (std::is_same_v<T, std::string>)
            {
                // O ImGui precisa de buffer para a manipulação de strings, pois ele
                // trabalha com char[]
                strncpy(objVal->InputBuffer, v.c_str(), sizeof(objVal->InputBuffer));
                if (ImGui::InputText(label, objVal->InputBuffer, sizeof(objVal->InputBuffer)))
                {
                    v = objVal->InputBuffer; // Atualização da string variant;
                }
            }

        }, objVal->Value);
    }

    void UINodeEditor::RenderBlueprintNode(Node* node)
    {
        ine::BeginNode(node->ID);

        // Header
        ImGui::Text("%s", node->Name.c_str());

        auto& io = ImGui::GetIO();

        ImGui::BeginGroup();
        {
            // -----------
            // Inputs
            // -----------
            ImGui::BeginGroup();
            {
                for (auto& input : node->Inputs)
                {
                    ine::BeginPin(input->ID, ine::PinKind::Input);
                    DrawPinIcon(*input.get(), IsSocketLinked(input->ID), 255.0f);
                    ine::EndPin();
                }
            }
            ImGui::EndGroup();

            // Mantém na mesma linha
            ImGui::SameLine();

            // -----------
            // Corpo
            // -----------
            ImGui::BeginGroup();
            {
                // Desativa os shortcuts enquanto algum componente estiver em foco.
                ine::EnableShortcuts(!io.WantTextInput);
                ImGui::PushItemWidth(150);

                for (auto& data : node->DataSet)
                {
                    NodeValue* objVal = data.second.get();

                    if (objVal->Type == SocketType::Int ||
                        objVal->Type == SocketType::Float ||
                        objVal->Type == SocketType::Bool ||
                        objVal->Type == SocketType::String)
                    {
                        RenderValue(data.first.c_str(), objVal);
                    }
                }

                ImGui::PopItemWidth();
            }
            ImGui::EndGroup();

            // Mantém na mesma linha
            ImGui::SameLine();

            // -----------
            // Outputs
            // -----------
            ImGui::BeginGroup();
            {
                for (auto& output : node->Outputs)
                {
                    ine::BeginPin(output->ID, ine::PinKind::Output);
                    DrawPinIcon(*output.get(), IsSocketLinked(output->ID), 255.0f);
                    ine::EndPin();
                }
            }
            ImGui::EndGroup();
        }
        ImGui::EndGroup();

        ine::EndNode();
    }

	void UINodeEditor::OnImGuiRender()
	{
        auto& io = ImGui::GetIO();

        // Criando links dinamicamente
        static ine::LinkId newLinkId;
        static ine::PinId startPinId, endPinId;

        ine::SetCurrentEditor(m_Context);

        ImGui::Begin(GetName().c_str());
        {
            ImGui::Text("FPS: %.2f (%.2gms)", io.Framerate, io.Framerate ? 1000.0f / io.Framerate : 0.0f);
            ImGui::Separator();

            const char* name = GetName().c_str();
            ine::Begin(name, ImVec2(0.0, 0.0f));
            {
                ine::SetCurrentEditor(m_Context);

                //-----------------------------
                // Renderiza os nodes
                //-----------------------------
                for (auto& node : m_Nodes)
                {
                    if (node.Type == NodeType::Blueprint)
                    {
                        RenderBlueprintNode(&node);
                    }
                }

                //-----------------------------
                // Renderiza os links
                //-----------------------------

                for (auto link : m_Links)
                {
                    ine::Link(link.ID, link.StartSocketId, link.EndSocketId);
                }

                // -----------------------------------------------------------------------
                //  Controles de comportamento de criação/exclusão de links entre nodes
                // -----------------------------------------------------------------------
                
                HandleLinkCreate();
                HandleLinkDelete();

                ine::End();
            }
            ImGui::End();
        }

        ine::SetCurrentEditor(nullptr);
	}
}