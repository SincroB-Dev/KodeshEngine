#pragma once

#include "Editor/UI/UIComponent.hpp"

#include <imgui/imgui.h>

namespace editor::ui
{
	/**
	 * @brief Componente responsável por inserir um espaço no layout.
	 **/
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