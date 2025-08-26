#pragma once

#include "Editor/Windows/UINodes/Drawing.hpp"

#include <imgui/imgui.h>

namespace editor::nodes::widgets
{
	using drawing::IconType;

	/**
	 * @brief Incluí o icone do socket na listagem de draw do imgui. 
	 **/
	void SocketIcon(const ImVec2& size, IconType type, bool filled, const ImVec4& color = ImVec4(1, 1, 1, 1), const ImVec4& innerColor = ImVec4(0, 0, 0, 0));
}