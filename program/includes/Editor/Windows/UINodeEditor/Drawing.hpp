#pragma once

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

namespace editor::nodes::drawing
{
	/**
	 * @brief Tipo do icone do socket.
	 **/
	enum class IconType: ImU32
	{
		Flow,
		Circle,
		Square,
		RoundSquare,
		Grid,
		Diamond
	};

	// ----------------------------------
	//  Funções de desenho encapsuladas
	// ----------------------------------

	void DrawFlow(ImDrawList* drawList, bool filled, ImVec2 rect, float rect_w, float outline_scale, ImColor color, ImColor innerColor);
	void DrawCircle(ImDrawList* drawList, bool filled, ImVec2 rect_center, float rect_w, float outline_scale, int extra_segments, ImColor color, ImColor innerColor);
	void DrawSquare(ImDrawList* drawList, bool filled, ImVec2 rect_center, float rect_w, float outline_scale, ImColor color, ImColor innerColor);
	void DrawRoundSquare(ImDrawList* drawList, bool filled, ImVec2 rect_center, float rect_w, float outline_scale, ImColor color, ImColor innerColor);
	void DrawGrid(ImDrawList* drawList, float& triangleStart, bool filled, ImVec2 rect_center, float rect_w, float rect_center_x, float rect_center_y, ImColor color);
	void DrawDiamond(ImDrawList* drawList, bool filled, ImVec2 rect_center, float rect_w, float outline_scale, ImColor color, ImColor innerColor);
	
	/**
	 * @brief Responsável por desenhar o icone correspondente ao tipo.
	 */
	void DrawIcon(ImDrawList* drawList, const ImVec2& a, const ImVec2& b, IconType type, bool filled, ImColor color, ImColor innerColor);

	inline ImVec2 ImRect_GetRect(const ImRect& rect)
	{
	    return ImVec2((rect.Min.x + rect.Max.x) * 0.5f,
	                  (rect.Min.y + rect.Max.y) * 0.5f);
	}

}