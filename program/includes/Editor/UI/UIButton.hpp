#include "Editor/UI/UIComponent.hpp"

#include <functional>

namespace editor::ui
{
	using UIButtonCallbackFn = std::function<void()>;

	/**
	 * @brief Componente responsável pelo display e callback de botões.
	 **/
	class UIButton : public UIComponent
	{
	public:
		template<typename... Func>
		UIButton(std::string label, Func&&... callbacks)
			: m_Label(label)
		{
			(m_Callbacks.emplace_back(std::forward<Func>(callbacks)), ...);
			m_IsEnabled = (sizeof...(callbacks)) > 0;
		}

		virtual void Click();

		/**
		 * @brief Registra um novo callback associado ao botão. Permite definir
		 *        ações que serão executadas quando o botão for acionado,
		 *        possibilitando o encadeamento de múltiplas operações.
		 */
		void PushCallback(UIButtonCallbackFn callback);

		/**
		 * @brief Limpa os a pilha de callbacks associados ao botão.
		 **/
		void ClearCallback();

		virtual void Render() override;

	protected:
		// Sequencia de callbacks que o componente ativa.
		std::vector<UIButtonCallbackFn> m_Callbacks;
		std::string m_Label;

		bool m_IsEnabled; // Desativa o botão caso seja false.
	};
}