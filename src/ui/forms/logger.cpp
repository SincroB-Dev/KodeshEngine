#include "logger.h"

#include "../../../libs/imgui/imgui.h"
#include "../../../libs/ImGuiFileDialog/ImGuiFileDialog.h"

#include "../materialIcons.h"

#include <stdexcept>
#include "../manager.h"

namespace core
{
    namespace ui
    {
        LogWindow* LogWindow::Instance = nullptr;
        
        LogWindow::LogWindow(): showLogWindow(true), isOpen(false), autoScroll(true),
            showInfo(true), showWarning(true), showError(true)
        {}
        
        LogWindow::~LogWindow()
        {}
        
        // Adiciona log
        void LogWindow::AddLog(const std::string& msg, LogType type) {
            logs.push_back({ msg, type });
            if (logs.size() > 1000) logs.erase(logs.begin());
        }

        // Mostra janela de logs avançada
        void LogWindow::Show() {
            // Caso a janela seja fechada.
            if (!isOpen)
            {
                return;
            }
            
            ImGui::Begin("Logs", &isOpen, ImGuiWindowFlags_MenuBar);

            // Menu de opções
            if (ImGui::BeginMenuBar()) {
                if (ImGui::BeginMenu("Opções")) {
                    if (ImGui::MenuItem("Limpar")) logs.clear();
                    if (ImGui::MenuItem(autoScroll ? "Parar rolagem" : "Auto-scroll")) autoScroll = !autoScroll;
                    ImGui::EndMenu();
                }
                ImGui::EndMenuBar();
            }
            
            ImGui::Checkbox("Info", &showInfo); ImGui::SameLine();
            ImGui::Checkbox("Warning", &showWarning); ImGui::SameLine();
            ImGui::Checkbox("Error", &showError);

            ImGui::Separator();

            ImGui::BeginChild("LogRegion", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);

            for (const auto& log : logs) {
                // Filtra por tipo
                if ((log.type == LogType::INFO && !showInfo) ||
                    (log.type == LogType::WARNING && !showWarning) ||
                    (log.type == LogType::ERROR && !showError))
                    continue;

                // Define cor e ícone
                ImVec4 color;
                const char* icon;
                switch (log.type) {
                    case LogType::INFO:    color = ImVec4(0.4f, 1.0f, 0.4f, 1.0f); icon = MICON_INFO; break;
                    case LogType::WARNING: color = ImVec4(1.0f, 0.9f, 0.0f, 1.0f); icon = MICON_WARNING; break;
                    case LogType::ERROR:   color = ImVec4(1.0f, 0.4f, 0.4f, 1.0f); icon = MICON_ERROR; break;
                }

                ImGui::PushStyleColor(ImGuiCol_Text, color);
                ImVec2 oldPadding = ImGui::GetStyle().FramePadding;
                ImGui::GetStyle().FramePadding.y = 4.0f; // ajuste fino
                ImGui::TextUnformatted((std::string(icon) + "  " + log.message).c_str());
                ImGui::GetStyle().FramePadding = oldPadding;
                ImGui::PopStyleColor();
            }

            // Rolagem automática
            if (autoScroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
                ImGui::SetScrollHereY(1.0f);

            ImGui::EndChild();
            ImGui::End();
        }

    }
}
