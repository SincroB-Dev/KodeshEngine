#include "Editor/UI/UIComponent.hpp"
#include "Editor/UI/UIButton.hpp"

#include <functional>
#include <vector>
#include <memory>

namespace core::systems
{
	class UILayer;
}

namespace editor::ui
{
	/**
	 * @brief Componente hierárquico para inclusão e gerenciamento de botões e submenus.
	 **/
	class UIMenuComponent : public UIComponent
	{
	public:
		template<typename... Func>
		UIMenuComponent(core::utils::UniqueID uid, 
			std::string label, std::string shortcut, Func&&... callbacks)
			: UIComponent(uid), Label(label), Shortcut(shortcut)
		{
			(m_Callbacks.emplace_back(std::forward<Func>(callbacks)), ...);
			IsEnabled = (sizeof...(callbacks)) > 0;
		}

		void Click();

		void PushCallback(UIButtonCallbackFn callback);
		void ClearCallback();

		void Render() override;

		bool IsEnabled;
		std::string Label;
		std::string Shortcut;


	private:
		// Sequencia de callbacks que o componente ativa.
		std::vector<UIButtonCallbackFn> m_Callbacks;
		std::vector<std::unique_ptr<UIComponent>> m_Childrens;

		friend class core::systems::UILayer;
	};
}