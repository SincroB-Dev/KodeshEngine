#include <ImGuiNodeEditor/imgui_node_editor.h>

#include "Core/Helpers/LogManager.hpp"
#include "Editor/Windows/UINodeEditor.hpp"

#include "Editor/UI/UITooltip.hpp"

#include "Editor/Windows/UINodeEditor/Interfaces/InputEventNode.hpp"

#include <iostream>
#include <algorithm>
#include <variant>
#include <string>

using namespace core::systems;
using namespace core::renderer;

namespace editor::nodes
{
	using namespace drawing;
    using namespace ui;

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
            LogManager::Log(LogType::Info, "Editor de nodes limpando context...");

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
	ImColor UINodeEditor::GetIconColor(core::MetaType socketType)
    {
        switch (socketType)
        {
            default:
            case core::MetaType::Flow:     return ImColor(255, 255, 255);
            case core::MetaType::Bool:     return ImColor(220,  48,  48);
            case core::MetaType::Int:      return ImColor( 68, 201, 156);
            case core::MetaType::Float:    return ImColor(147, 226,  74);
            case core::MetaType::String:   return ImColor(124,  21, 153);
            case core::MetaType::Object:   return ImColor( 51, 150, 215);
            case core::MetaType::Function: return ImColor(218,   0, 183);
            case core::MetaType::Delegate: return ImColor(255,  48,  48);
        }
    };

    void UINodeEditor::DrawPinIcon(const Socket& socket, bool connected, int alpha)
    {
        IconType iconType;
        ImColor  color = GetIconColor(socket.Type);
        color.Value.w = alpha / 255.0f;
        switch (socket.Type)
        {
            case core::MetaType::Flow:     iconType = IconType::Flow;   break;
            case core::MetaType::Bool:     iconType = IconType::Circle; break;
            case core::MetaType::Int:      iconType = IconType::Circle; break;
            case core::MetaType::Float:    iconType = IconType::Circle; break;
            case core::MetaType::String:   iconType = IconType::Circle; break;
            case core::MetaType::Object:   iconType = IconType::Circle; break;
            case core::MetaType::Function: iconType = IconType::Circle; break;
            case core::MetaType::Delegate: iconType = IconType::Square; break;
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
            if (node->ID == id)
                return node.get();

        return nullptr;
    }

    Link* UINodeEditor::FindLink(ine::LinkId id)
    {
        for (auto& link : m_Links)
            if (link->ID == id)
                return link.get();

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
            if (link->Output->ID == id || link->Input->ID == id)
                return true;

        return false;
    }

    bool UINodeEditor::CanCreateLink(Socket*& a, Socket*& b)
    {
        if (!a || !b || a == b ||     // Verifica se são nulos ou iguais
            a->Kind == b->Kind ||     // Verifica se os tipos são diferentes (input/output)
            a->Type != b->Type ||     // Verifica se carregam os mesmos tipos de dados
            a->NodePtr == b->NodePtr  // Verifica se apontam para o mesmo proprietário
        )
        {
            return false;
        }

        // Garantia de que a sempre será o socket de saída e b sempre será o socket de entrada.
        if (a->Kind == SocketKind::Input)
        {
            std::swap(a, b);
        }

        return true;
    }

    //----------------------------------------
    // Comportamentos em create, delete link
    //----------------------------------------

    void UINodeEditor::HandleLinkCreate()
    {
        if (ine::BeginCreate())
        {
            ine::PinId pin1, pin2;

            if (ine::QueryNewLink(&pin1, &pin2, ImVec4(0.4f, 1.0f, 0.1f, 1.0f)))
            {
                if (pin1 && pin2)
                {
                    if (ine::AcceptNewItem())
                    {
                        Socket* output = FindSocket(pin1);
                        Socket* input = FindSocket(pin2);

                        if (CanCreateLink(output, input))
                        {
                            m_Links.push_back(std::make_unique<Link>(GetNextId(), output, input));
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
                            [deletedLinkId](const std::unique_ptr<Link>& link) {
                                return link->ID == deletedLinkId;
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
        // Caso seja um enum, ele vai construir um botão para assumir valores.
        if (objVal->Type == core::MetaType::Enum && objVal->Descriptor != nullptr)
        {
            ImGui::PushID(&objVal);
            
            if (ImGui::BeginCombo("", EnumToString(*objVal->Descriptor, objVal->GetIntValue()))) 
            {
                widgets::UINodePopup::OpenPopup(
                    [objVal]()
                    {
                        for (auto& e : objVal->Descriptor->EnumEntries)
                        {
                            if (ImGui::Selectable(e.Name, e.Value == objVal->GetIntValue()))
                            {
                                objVal->Value = e.Value;
                                widgets::UINodePopup::Close();
                            }
                        }
                    }, 1
                );
                ImGui::CloseCurrentPopup();
                ImGui::EndCombo();
            }

            ImGui::PopID();
        }

        // Caso seja outros tipos primitivos.
        else
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

        // Faz a tooltip
        if (!objVal->Help.empty())
        {
            if (ImGui::IsItemHovered())
            {
                UITooltip::SetTooltip(objVal->Help);
            }
        }
    }

    void UINodeEditor::RenderBlueprintNode(Node* node)
    {
        auto& io = ImGui::GetIO();

        ine::BeginNode(node->ID);

        // Header
        ImGui::BeginGroup();
        {
            ImGui::TextUnformatted(node->Name.c_str());
            ImGui::Dummy(ImVec2(0.0f, 5.0f));
        }
        ImGui::EndGroup();

        ImGui::BeginGroup();
        {
            // -----------
            // Inputs
            // -----------

            // Para correção no layout, sempre que não houver input nodes, adiciona um Spacing, assim ele ajusta automaticamente o espacamento entre os nodes de saída.
            if (node->Inputs.size() > 0)
            {
                ImGui::BeginGroup();
                {
                    for (auto& input : node->Inputs)
                    {
                        ImGui::PushItemWidth(40);
                        ine::BeginPin(input->ID, ine::PinKind::Input);
                        {
                            // Desenha o pin, futuramente encaixar o nome dele com mais calma
                            DrawPinIcon(*input.get(), IsSocketLinked(input->ID), 255.0f);
                        }
                        ine::EndPin();
                        ImGui::PopItemWidth();
                    }
                }
                ImGui::EndGroup();

            }
            else
            {
                ImGui::Spacing();
            }

            // Mantém na mesma linha
            ImGui::SameLine();

            // -----------
            // Corpo
            // -----------

            // Marca se o corpo está vazio, se estiver preenche com Dummy, evitando desajuste de layout
            bool isEmptyBody = true;

            ImGui::BeginGroup();
            {
                // Desativa os shortcuts enquanto algum componente estiver em foco
                // Deve ser chamado pois o NodeEditor bloqueia todas as entradas de teclas
                ine::EnableShortcuts(!io.WantTextInput);

                ImGui::PushItemWidth(150);

                // Verifica se o objeto do é tipo de recebimento de input, caso seja adiciona
                // seu botão correspondente.
                if (node->IsAnyType(NodeType::InputEvent))
                {
                    ((compositor::InputEventNode*)node)->Button.Render();

                    // Notifica que não precisa do dummy
                    isEmptyBody = false;
                }

                for (auto& data : node->DataSet)
                {
                    NodeValue* objVal = data.second.get();

                    // Verifica como prioridade as propriedades anexadas ao node
                    if (objVal->Type == core::MetaType::Int ||
                        objVal->Type == core::MetaType::Float ||
                        objVal->Type == core::MetaType::Bool ||
                        objVal->Type == core::MetaType::String ||
                        objVal->Type == core::MetaType::Enum)
                    {
                        RenderValue(data.first.c_str(), objVal);

                        // Notifica que não precisa do dummy
                        isEmptyBody = false;
                    }
                }

                if (isEmptyBody)
                {
                    // Importante para evitar desajuste de layout
                    ImGui::Dummy(ImVec2(150, 0));
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
                    if (node->IsAnyType(NodeType::Blueprint))
                    {
                        RenderBlueprintNode(node.get());
                    }
                }

                //-----------------------------
                // Renderiza os links
                //-----------------------------

                for (auto& link : m_Links)
                {
                    ine::Link(link->ID, link->Output->ID, link->Input->ID);
                }

                //---------------------------------
                // Renderiza menus-popup/tooltips
                //---------------------------------


                // Inicio de seção diferida.
                ine::Suspend();
                {
                    // Renderização de popups.
                    widgets::UINodePopup::Render();

                    // Renderização da tooltip.
                    UITooltip::Render();
                }
                ine::Resume();
                // Fim de seção diferida.

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