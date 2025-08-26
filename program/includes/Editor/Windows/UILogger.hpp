#pragma once

#include "Editor/Windows/UIWindow.hpp"

namespace editor::windows
{
	class UILogger : public UIWindow
	{
	public:
		UILogger()
			: UIWindow("-- [Kodesh Logger] --"),
			m_AutoScroll(true), 
			m_ShowInfo(true), m_ShowWarning(true), m_ShowError(true), 
			m_ShowEDebug(true) // Especial, só entra no modo de depuração.
		{}

		void OnImGuiRender() override;

	private:
		bool m_AutoScroll;
		bool m_ShowInfo;
		bool m_ShowWarning;
		bool m_ShowError;
		bool m_ShowEDebug;
	};
}