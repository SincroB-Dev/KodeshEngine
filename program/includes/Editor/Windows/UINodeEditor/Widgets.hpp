#pragma once

#include "Editor/Windows/UINodeEditor/Drawing.hpp"

#include <imgui/imgui.h>
#include <functional>

namespace editor::nodes::widgets
{
	using drawing::IconType;

	/**
	 * @brief Incluí o icone do socket na listagem de draw do imgui. 
	 **/
	void SocketIcon(const ImVec2& size, IconType type, bool filled, const ImVec4& color = ImVec4(1, 1, 1, 1), const ImVec4& innerColor = ImVec4(0, 0, 0, 0));

	/**
	 * @brief Abstração de função para melhor leitura de código.
	 **/
	using UINodePopupFn = std::function<void()>;

	/**
	 * @brief Devido o problema de comportamento do node com comboboxes, e outros itens, 
	 * 		  este construtor de popup foi necessário.
	 **/
	struct UINodePopup
	{
		/**
		 * @brief Apresenta um text como tooltip.
		 **/
		static void OpenPopup(UINodePopupFn fn, int popupId);

		/**
		 * @brief Chama a função de close current popup do imgui.
		 **/
		static void Close();

		/**
		 * @brief Deve ser chamado para a renderização do popup.
		 **/
		static void Render();

	private:
		static inline UINodePopupFn s_PopupFn = nullptr;
		static inline int s_PopupId = -1;
	};
}