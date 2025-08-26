#include "Editor/Windows/UINodeEditor/Widgets.hpp"
#include "Core/Helpers/ImGuiOperators.hpp"

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
}