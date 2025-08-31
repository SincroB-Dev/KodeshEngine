#include "Editor/UI/UITooltip.hpp"
#include <imgui/imgui.h>

namespace editor::ui
{
	void UITooltip::SetTooltip(std::string text)
	{
		SetTooltip(
			[text](){
            	ImGui::Text("%s", text.c_str());
			}
		);
	}

	void UITooltip::SetTooltip(UITooltipFn complex)
	{
		s_DrawFn = std::move(complex);
	}

	void UITooltip::Render()
	{
		if (s_DrawFn && ImGui::IsAnyItemHovered()) 
		{
            ImGui::BeginTooltip();
            {
            	s_DrawFn();
            }
            ImGui::EndTooltip();
        }

        s_DrawFn = nullptr; // reseta a cada frame
	}
}