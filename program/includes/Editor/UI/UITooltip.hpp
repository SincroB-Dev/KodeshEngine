#pragma once

#include <memory>
#include <functional>

namespace editor::ui
{
	using UITooltipFn = std::function<void()>;

	/**
	 * @brief Este controlador de tooltip, a primeiro momento, foi criado para contornar um problema
	 * 	   	  com o NodeEditor, o qual no suporta trabalhar com tooltips dentro de nodes. 
	 **/
	struct UITooltip
	{
		/**
		 * @brief Apresenta um text como tooltip.
		 **/
		static void SetTooltip(std::string text);

		/**
		 * @brief Permite a criação de um tooltip mais complexo.
		 **/
		static void SetTooltip(UITooltipFn complex);

		/**
		 * @brief Deve ser chamado para a renderização dos tooltips.
		 **/
		static void Render();

	private:
		static inline UITooltipFn s_DrawFn = nullptr;

		UITooltip() {};
	};
}