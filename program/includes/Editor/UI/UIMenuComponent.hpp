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
	class UIMenuComponent : public UIButton
	{
	public:
		template<typename... Func>
		UIMenuComponent(std::string label, std::string shortcut, Func&&... callbacks)
			: UIButton(label, std::forward<Func>(callbacks)...), m_Shortcut(shortcut)
		{}

		/**
		 * @brief Metodo utilizado para a chamada de funções empilhadas em m_Callbacks,
		 * 		  somente não é disparado caso m_Childrens tenha algum item.
		 **/
		void Click() override;

		void Render() override;

	private:
		std::vector<std::unique_ptr<UIComponent>> m_Childrens; // Filhos deste botão, assim é possível renderizar um submenu hierarquico

		std::string m_Shortcut;

		friend class core::systems::UILayer;
	};
}