#include "environmentInfoPanel.h"

#include "../../../libs/imgui/imgui.h"
#include "../../../libs/ImGuiFileDialog/ImGuiFileDialog.h"

namespace core
{
    namespace ui
    {
        EnvironmentPanel::EnvironmentPanel(SceneManager &sm): Panel(sm)
        {}
        
        EnvironmentPanel::~EnvironmentPanel()
        {}
        
        void EnvironmentPanel::CameraInfo()
        {
            Camera2D* camera = sceneManager.GetActiveCamera();
            
            // Posição
            if (ImGui::CollapsingHeader("Camera")) {
                if (ImGui::BeginTable("CameraTransformationTable", 2)) {
                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0); ImGui::Text("Pos X:");
                    ImGui::TableSetColumnIndex(1); ImGui::DragFloat("##CAMPX", &camera->position.x, 0.1f);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0); ImGui::Text("Pos Y:");
                    ImGui::TableSetColumnIndex(1); ImGui::DragFloat("##CAMPY", &camera->position.y, 0.1f);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0); ImGui::Text("Zoom (Space):");
                    ImGui::TableSetColumnIndex(1); ImGui::DragFloat("##CAMSP", &camera->space, 0.1f);

                    ImGui::EndTable();
                }
            }
        }
        
        void EnvironmentPanel::drawMenu()
        {
        }
        
        void EnvironmentPanel::drawPanel(void* properties)
        {
            CameraInfo();
        }
    }
}
