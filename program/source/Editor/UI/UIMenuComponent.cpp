#include "Editor/UI/UIMenuComponent.hpp"

#include <imgui/imgui.h>

namespace editor
{
	namespace ui
	{
		void UIMenuComponent::Click()
		{
			if (!IsEnabled || m_Childrens.size() > 0)
			{
				return;
			}

			for (UIButtonCallbackFn& callback : m_Callbacks)
			{
				callback();
			}
		}

		void UIMenuComponent::PushCallback(UIButtonCallbackFn callback)
		{
			m_Callbacks.push_back(callback);
			IsEnabled = true;
		}

		void UIMenuComponent::ClearCallback()
		{
			m_Callbacks.clear();
			IsEnabled = false;
		}

		void UIMenuComponent::Render()
		{
			ImGui::PushID(UID.ID);
			if (m_Childrens.size() > 0)
			{
				if (ImGui::BeginMenu(Label.c_str()))
                {
            		for (auto& child : m_Childrens)
            		{
            			child->Render();
            		}

                	ImGui::EndMenu();
                }
                goto CloseComponent;
			}

			if (!IsEnabled)
			{
				ImGui::BeginDisabled();
			}

			if (ImGui::MenuItem(Label.c_str(), Shortcut.c_str())) 
			{
				Click();
            }

			if (!IsEnabled)
			{
				ImGui::EndDisabled();
			}

		CloseComponent:
			ImGui::PopID();
		}
	}
}