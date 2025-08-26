#include "Editor/UI/UIButton.hpp"

namespace editor::ui
{
	void UIButton::Click()
	{
		for (UIButtonCallbackFn& callback : m_Callbacks)
		{
			callback();
		}
	}

	void UIButton::PushCallback(UIButtonCallbackFn callback)
	{
		m_Callbacks.push_back(callback);
		m_IsEnabled = true;
	}

	void UIButton::ClearCallback()
	{
		m_Callbacks.clear();
		m_IsEnabled = false;
	}

	void UIButton::Render()
	{
		// Renderização do botão
	}
}