#include "Editor/UI/UIMenuComponent.hpp"

#include <imgui/imgui.h>

namespace editor::ui
{
	void UIMenuComponent::Click()
	{
		if (!m_IsEnabled || m_Childrens.size() > 0)
		{
			return;
		}

		for (UIButtonCallbackFn& callback : m_Callbacks)
		{
			callback();
		}
	}

	void UIMenuComponent::Render()
	{
		ImGui::PushID(this);
		if (m_Childrens.size() > 0)
		{
			if (ImGui::BeginMenu(m_Label.c_str()))
            {
        		for (auto& child : m_Childrens)
        		{
        			child->Render();
        		}

            	ImGui::EndMenu();
            }
            goto CloseComponent;
		}

		if (!m_IsEnabled)
		{
			ImGui::BeginDisabled();
		}

		if (ImGui::MenuItem(m_Label.c_str(), m_Shortcut.c_str())) 
		{
			Click();
        }

		if (!m_IsEnabled)
		{
			ImGui::EndDisabled();
		}

	CloseComponent:
		ImGui::PopID();
	}
}