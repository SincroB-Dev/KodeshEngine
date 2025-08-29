#include "Core/Helpers/LogManager.hpp"
#include "Editor/Windows/UILogger.hpp"

#include "Editor/UI/Fontes/MaterialIcons.hpp"

// Excluir
#include "Core/Utils/UniqueIDGen.hpp"
#include "Editor/UI/UIPressButton.hpp"

#include <imgui/imgui.h>

using namespace core::systems;

namespace editor::windows
{
	void UILogger::OnImGuiRender()
	{
		if (!IsOpen()) return;

		ImGui::Begin(GetName().c_str(), &m_IsOpen, ImGuiWindowFlags_MenuBar);

		// Caso o logger não tenha sido instanciado, a janela
		// informa que não tem logger disponível.
		if (!LogManager::HasLogger())
		{
            ImGui::TextUnformatted("Nenhuma instancia de logs definida.");

			ImGui::End();
			return;
		}

        // Menu de opções
        if (ImGui::BeginMenuBar()) {
            if (ImGui::BeginMenu("Opções")) {
                if (ImGui::MenuItem("Limpar")) { /*logs.clear();*/ }
                if (ImGui::MenuItem(m_AutoScroll ? "Parar rolagem" : "Auto-scroll")) m_AutoScroll = !m_AutoScroll;
                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }
        
        ImGui::Checkbox("Info", &m_ShowInfo); ImGui::SameLine();
        ImGui::Checkbox("Warning", &m_ShowWarning); ImGui::SameLine();
        ImGui::Checkbox("Error", &m_ShowError); ImGui::SameLine();
        ImGui::Checkbox("Editor Debug", &m_ShowEDebug);

        ImGui::Separator();

        ImGui::BeginChild("-- [Logs] --", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);

        for (const auto& log : LogManager::GetLogs()) {
            // Filtra por tipo
            if ((log.Type == LogType::Info && !m_ShowInfo) ||
                (log.Type == LogType::Warning && !m_ShowWarning) ||
                (log.Type == LogType::Error && !m_ShowError) ||
                (log.Type == LogType::EDebug && !m_ShowEDebug))
                continue;

            // Define cor e ícone
            ImVec4 color;
            const char* icon;
            switch (log.Type) {
                case LogType::Info:    color = ImVec4(0.4f, 1.0f, 0.4f, 1.0f); icon = MICON_INFO; break;
                case LogType::Warning: color = ImVec4(1.0f, 0.9f, 0.0f, 1.0f); icon = MICON_WARNING; break;
                case LogType::Error:   color = ImVec4(1.0f, 0.4f, 0.4f, 1.0f); icon = MICON_ERROR; break;
                case LogType::EDebug:   color = ImVec4(1.0f, 0.4f, 0.4f, 1.0f); icon = MICON_BUG_REPORT; break;
            }

            ImGui::PushStyleColor(ImGuiCol_Text, color);
            ImVec2 oldPadding = ImGui::GetStyle().FramePadding;
            ImGui::GetStyle().FramePadding.y = 4.0f; // ajuste fino
            ImGui::TextUnformatted((std::string(icon) + "  " + log.Message).c_str());
            ImGui::GetStyle().FramePadding = oldPadding;
            ImGui::PopStyleColor();
        }

        // Rolagem automática
        if (m_AutoScroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
        {
            ImGui::SetScrollHereY(1.0f);
        }

        ImGui::EndChild();
        ImGui::End();
	}
}