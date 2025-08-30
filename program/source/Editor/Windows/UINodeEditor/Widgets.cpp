#include "Editor/Windows/UINodeEditor/Widgets.hpp"
#include "Core/Maths/ImGuiOperators.hpp"

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

namespace editor::nodes::widgets
{
	void SocketIcon(const ImVec2& size, IconType type, bool filled, const ImVec4& color, const ImVec4& innerColor)
	{
	    if (ImGui::IsRectVisible(size))
	    {
	        auto cursorPos = ImGui::GetCursorScreenPos();
	        auto drawList  = ImGui::GetWindowDrawList();
	        
	        drawing::DrawIcon(drawList, cursorPos, cursorPos + size, type, filled, ImColor(color), ImColor(innerColor));
	    }

	    ImGui::Dummy(size);
	}

	//------------------------------------
	// Popup
	//------------------------------------

	void UINodePopup::OpenPopup(UINodePopupFn fn, int popupId)
	{
		s_PopupId = popupId;
		s_PopupFn = std::move(fn);
	}

	void UINodePopup::Close()
	{
		ImGui::CloseCurrentPopup(); 

		// Limpa a função de popup
		s_PopupFn = nullptr;
	}

	void UINodePopup::Render()
	{
		if (s_PopupId != -1)
		{
		    ImGui::OpenPopup("NodeEditorPopup");
		    s_PopupId = -1; // evita reabrir no próximo frame
		}

		if (ImGui::BeginPopup("NodeEditorPopup")) 
		{
		    if (s_PopupFn) 
    		{
		    	s_PopupFn();
    		}

		    ImGui::EndPopup();
		}
	}
}