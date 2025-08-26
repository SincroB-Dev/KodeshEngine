#pragma once

#include "Core/Utils/UniqueIDGen.hpp"

#include <string>
#include <functional>

namespace editor::ui
{
	/**
	 * @brief Abstração para utilização em interfaces.
	 */
	class UIComponent
	{
	public:
		virtual ~UIComponent() = default;

		/**
		 * @brief Método padrão de renderização de componentes.
		 **/
		virtual void Render() = 0;
	};
}