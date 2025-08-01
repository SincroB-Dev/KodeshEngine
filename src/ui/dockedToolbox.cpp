#include "dockedToolbox.h"

#include "../../libs/imgui/imgui.h"

namespace core
{
    namespace ui
    {
        void DockedToolbox::Draw()
        {
            enum class EditorTab
            {
                Ambiente,
                Cenario,
                Entidade,
                Material,
                Componentes
            };

            static EditorTab activeTab = EditorTab::Ambiente;

            static float panelWidth = 200.0f;
            const float panelWidthMin = 200.0f;
            const float panelWidthMax = 500.0f;

            // Obter tamanho da janela principal
            ImVec2 viewportSize = ImGui::GetMainViewport()->Size;

            // Calcular posição do painel na lateral direita
            float panelX = viewportSize.x - panelWidth;
            ImVec2 panelPos(panelX, 0);
            ImVec2 panelSize(panelWidth, viewportSize.y);

            // Configurar posição e tamanho do painel
            ImGui::SetNextWindowPos(panelPos, ImGuiCond_Always);
            ImGui::SetNextWindowSize(panelSize, ImGuiCond_Always);
            ImGui::SetNextWindowBgAlpha(1.0f);

            ImGuiWindowFlags flags = ImGuiWindowFlags_NoTitleBar |
                                     ImGuiWindowFlags_NoCollapse |
                                     ImGuiWindowFlags_NoMove |
                                     ImGuiWindowFlags_NoBringToFrontOnFocus |
                                     ImGuiWindowFlags_NoNavFocus;

            if (ImGui::Begin("RightPanel", nullptr, flags))
            {
                ImVec2 panelSize = ImGui::GetContentRegionAvail();

                float iconButtonSize = 32.0f; // Botões quadrados
                float tabColumnWidth = iconButtonSize + 4.0f;

                ImGui::BeginChild("TabButtons", ImVec2(tabColumnWidth, panelSize.y), true);

                auto DrawIconButton = [&](const char *icon, EditorTab tab)
                {
                    bool selected = (activeTab == tab);
                    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 2.0f); // opcional, deixa cantos arredondados

                    if (selected)
                        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.3f, 0.4f, 0.6f, 1.0f)); // Cor de seleção

                    if (ImGui::Button(icon, ImVec2(iconButtonSize, iconButtonSize)))
                        activeTab = tab;

                    if (selected)
                        ImGui::PopStyleColor();

                    ImGui::PopStyleVar(1);
                };

                DrawIconButton(u8"\ue80b", EditorTab::Ambiente);    // 🌐 Ambiente
                DrawIconButton(u8"\ue3f4", EditorTab::Cenario);     // 🖼️ Cenário
                DrawIconButton(u8"\ue7fd", EditorTab::Entidade);    // 👤 Entidade
                DrawIconButton(u8"\ue86c", EditorTab::Material);    // 🧱 Material
                DrawIconButton(u8"\ue87b", EditorTab::Componentes); // 🧩 Componentes

                ImGui::EndChild();

                // Conteúdo da aba selecionada
                ImGui::SameLine();
                ImGui::BeginChild("TabContent", ImVec2(0, panelSize.y), false);

                switch (activeTab)
                {
                case EditorTab::Ambiente:
                    ImGui::Text("Configurações do Ambiente");
                    break;
                case EditorTab::Cenario:
                    ImGui::Text("Elementos do Cenário");
                    break;
                case EditorTab::Entidade:
                    ImGui::Text("Lista de Entidades");
                    break;
                case EditorTab::Material:
                    ImGui::Text("Editor de Materiais");
                    break;
                case EditorTab::Componentes:
                    ImGui::Text("Componentes disponíveis");
                    break;
                }

                ImGui::EndChild();
            }
            ImGui::End();
        }
    };
};