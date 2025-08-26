#include "Editor/UI/UIComponent.hpp"

#include <functional>

namespace editor
{
	namespace ui
	{
		using UIButtonCallbackFn = std::function<void()>;

		/**
		 * @brief Componente responsável pelo display e callback de botões.
		 **/
		class UIButton : public UIComponent
		{
		public:
			template<typename... Func>
			UIButton(core::utils::UniqueID uid, std::string label, Func&&... callbacks)
				: UIComponent(uid), m_Label(label)
			{
				(m_Callbacks.emplace_back(std::forward<Func>(callbacks)), ...);
				IsEnabled = (sizeof...(callbacks)) > 0;
			}

			void Click();

			void PushCallback(UIButtonCallbackFn callback);
			void ClearCallback();

			virtual void Render() override;

			bool IsEnabled;

		private:
			// Sequencia de callbacks que o componente ativa.
			std::vector<UIButtonCallbackFn> m_Callbacks;
			std::string m_Label;
		};
	}
}