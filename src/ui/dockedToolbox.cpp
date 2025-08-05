#include "dockedToolbox.h"

#include "../../libs/imgui/imgui.h"
#include "materialIcons.h"

#include <iostream>

namespace core
{
    namespace ui
    {
        void DockedToolbox::DrawIconButton(const char *icon, TBEditorTab tab)
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
        }

        void DockedToolbox::Draw()
        {
            ImVec2 viewportSize = ImGui::GetMainViewport()->Size;
            float panelX = viewportSize.x - panelWidth;
            ImVec2 panelPos(panelX, 0);
            ImVec2 panelSize(panelWidth, viewportSize.y);

            ImGui::SetNextWindowPos(panelPos, ImGuiCond_Always);
            ImGui::SetNextWindowSize(panelSize, ImGuiCond_Always);
            ImGui::SetNextWindowBgAlpha(1.0f);

            ImGuiWindowFlags flags = ImGuiWindowFlags_NoTitleBar |
                                     ImGuiWindowFlags_NoMove |
                                     ImGuiWindowFlags_NoBringToFrontOnFocus |
                                     ImGuiWindowFlags_NoNavFocus |
                                     ImGuiWindowFlags_NoResize;

            ImGui::Begin("RightPanel", nullptr, flags);

            ImGui::SameLine();
            ImGui::SetCursorPosX(12);
            ImGui::BeginChild("TabButtons", ImVec2(tabColumnWidth, panelSize.y), true);

            DrawIconButton(MICON_FOREST, TBEditorTab::Ambiente);
            DrawIconButton(MICON_SCENE, TBEditorTab::Cenario);
            DrawIconButton(MICON_DEPLOYED_CODE, TBEditorTab::Entidade);
            DrawIconButton(MICON_FORMAT_PAINT, TBEditorTab::Material);
            DrawIconButton(MICON_COMPONENT_EXCHANGE, TBEditorTab::Componentes);

            ImGui::EndChild();

            ImGui::SameLine();
            ImGui::BeginChild("EnvironmentManagerTabs", ImVec2(0, panelSize.y), false);

            switch (activeTab)
            {
            case TBEditorTab::Ambiente:
                ImGui::Text("Configurações do Ambiente");
                break;
            case TBEditorTab::Cenario:
                ImGui::Text("Elementos do Cenário");
                break;
            case TBEditorTab::Entidade:
                ImGui::Text("Configurações da Entidade Selecionada");
                break;
            case TBEditorTab::Material:
                ImGui::Text("Editor de Materiais");
                break;
            case TBEditorTab::Componentes:
                ImGui::Text("Componentes disponíveis");
                break;
            }

            ImGui::EndChild();
            ImGui::End();
        }
    };
};