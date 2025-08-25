#pragma once

#include "Editor/UI/UIComponent.hpp"

#include <imgui/imgui.h>

namespace editor
{
	namespace ui
	{
		class UISeparator : public UIComponent
		{
		public:
			UISeparator(): UIComponent(0) {}

			void Render() override
			{
				ImGui::Separator();
			}
		};
	}
}