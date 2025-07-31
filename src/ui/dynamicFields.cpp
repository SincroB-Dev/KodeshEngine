#include "dynamicFields.h"
#include <iostream>

void DynamicFields(Serializable *obj) 
{
    for (const auto &field : obj->GetFields())
    {
        if (field.type == typeid(int))
        {
            ImGui::InputInt(field.name.c_str(), static_cast<int*>(field.ptr));
        }
        else if (field.type == typeid(float))
        {
            ImGui::InputFloat(field.name.c_str(), static_cast<float*>(field.ptr));
        }
        else if (field.type == typeid(vec2))
        {
            ImGui::InputFloat2(field.name.c_str(), (float*)field.ptr);
        }
        else if (field.type == typeid(Color))
        {
            ImGui::ColorEdit3(field.name.c_str(), (float*)field.ptr);
        }
        else if (field.type == typeid(std::string))
        {
            ImGui::InputText(field.name.c_str(), static_cast<char*>(field.ptr), IM_ARRAYSIZE(&field.ptr));
        }
        else if (field.type == typeid(bool))
        {
            ImGuiStyle& style = ImGui::GetStyle();

            ImVec2 originalPadding = style.FramePadding;

            // Reduz temporariamente o padding
            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2.0f, 2.0f)); // menor padding

            ImGui::Checkbox(field.name.c_str(), (bool*)field.ptr);

            ImGui::PopStyleVar();
        }
        else
        {
            ImGui::Text("%s não pôde ser incluso!", field.name.c_str());
        }
    }
}