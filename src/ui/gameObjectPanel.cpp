#include "gameObjectPanel.h"
#include "../core/core.h"

#include "../../libs/imgui/imgui.h"
#include "../../libs/imgui/backends/imgui_impl_sdl2.h"
#include "../../libs/imgui/backends/imgui_impl_opengl2.h"

#include "dynamicFields.h"

GameObjectPanel::GameObjectPanel()
{

}

GameObjectPanel::~GameObjectPanel()
{

}

void GameObjectPanel::drawPanel(void *properties)
{
    ImGui::Begin("Propriedades de Objeto");

    if (properties)
    {
        GameObject *props = (GameObject*) properties;

        // Propriedades do objeto
        ImGui::Text("Name: %s", props->name);
        ImGui::Spacing();

        // Posição
        if (ImGui::CollapsingHeader("Transformação")) {
            ImGui::Text("Posição:");

            if (ImGui::BeginTable("TransformationTable", 2)) {
                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0); ImGui::Text("Pos X:");
                ImGui::TableSetColumnIndex(1); ImGui::DragFloat("##PX", &props->transform.localPosition.x, 0.1f);

                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0); ImGui::Text("Pos Y:");
                ImGui::TableSetColumnIndex(1); ImGui::DragFloat("##PY", &props->transform.localPosition.y, 0.1f);

                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0); ImGui::Text("Escala X:");
                ImGui::TableSetColumnIndex(1); ImGui::DragFloat("##SX", &props->transform.localScale.x, 0.1f);

                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0); ImGui::Text("Escala Y:");
                ImGui::TableSetColumnIndex(1); ImGui::DragFloat("##SY", &props->transform.localScale.y, 0.1f);

                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0); ImGui::Text("Rotação (graus):");
                ImGui::TableSetColumnIndex(1); ImGui::DragFloat("##A", &props->transform.localAngle, 0.1f, -360.0f, 360.0f);

                ImGui::EndTable();
            }
        }
        
        // Caso o objeto possua uma configuração aplicada de shape
        if (props->shape != nullptr)
        {    
            // Propriedades do Shape
            if (ImGui::CollapsingHeader("Shape Props"))
            {
                DynamicFields(props->shape);
            }
        }
        else
        {
            ImGui::Button("Adicionar Shape");
        }
    }
    else
    {
        ImGui::Text("Nenhum objeto selecionado!");
    }

    ImGui::End();
}