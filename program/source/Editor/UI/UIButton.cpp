#include "Editor/UI/UIButton.hpp"

namespace editor
{
	namespace ui
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
			IsEnabled = true;
		}

		void UIButton::ClearCallback()
		{
			m_Callbacks.clear();
			IsEnabled = false;
		}

		void UIButton::Render()
		{
			// Renderização do botão
		}
	}
}