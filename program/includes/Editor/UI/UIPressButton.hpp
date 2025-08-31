#pragma once

#include "Editor/UI/UIComponent.hpp"
#include "Core/Utils/UniqueIDGen.hpp"
#include "Core/Utils/Metadata.hpp"

#include <functional>
#include <array>

namespace editor::ui
{
	/**
	 * @brief Deixa o tipo de UIPressButton mais genérico, facilitando a chamada.
	 **/
	enum class UserInputEnum
	{
		Keyboard    = 0, 
		MouseButton = 1, 
		JoyButton   = 2
	};

	/**
	 * @brief Componente de interface responsável por aguardar a captura de um evento (Teclado/Mouse)
	 *        de entrada do usuário e armazena-lo para uso posterior.
	 **/
	class UIPressButton : public UIComponent
	{
	public:
		UIPressButton(UserInputEnum type,
					  std::string emptyLabel, std::string awaitingLabel,
					  int defaultCode = -1)
			: m_EmptyLabel(emptyLabel), m_AwaitingLabel(awaitingLabel),
			  m_Type(type), m_IsAwaiting(false)
		{
			// Necessita ser chamado para a inclusão do título de captura.
			SetCaptured(defaultCode);
		}

		~UIPressButton()
		{
			// Impede um dangling pointer (ponteiro pendente)
			if (this == s_CurrentAwaiting)
			{
				ResetButtonState();
			}
		}

		/**
		 * @brief Retorna o código de acionamento do evento. 
		 **/
		int GetCode() const { return m_Code; }

		/**
		 * @brief Retorna o título de captura do evento.
		 **/
		const char* GetLabel() const { return GetCode() > 0 ? m_Captured.c_str() : "<Empty>"; }

		/**
		 * @brief Retorna se o botão está aguardando alguma entrada.
		 **/
		bool const IsAwaiting() const { return m_IsAwaiting; }

		/**
		 * @brief Seta a tecla capturada, usada até mesmo para restaurar o estado salvo do botão. 
		 **/
		void SetCaptured(int code = -1);
		
		/**
		 * @brief Faz um reset em propriedades essenciais do botão, sendo m_IsAwaiting, m_Code, 
		 * 		  e também reseta a singletron s_CurrentAwaiting.
		 **/
		void ResetButtonState();

		/**
		 * @brief Renderiza o botão, quando m_IsAwaiting está true, ele vai permanecer com
		 * 		  cor de ativo do botão.
		 **/
		virtual void Render() override;

		/**
		 * @brief Comportamento do botão em estado ativo, ou seja, m_IsAwaiting true.
		 *        Aqui ele aguarda uma entrada de botão/tecla efetuada pelo usuário.
		 **/
		void HandlePressButton();


	private:
		std::string m_EmptyLabel; // Texto do botão quando está sem captura
		std::string m_AwaitingLabel; // Texto do botão quando está esperando por uma captura
		std::string m_Captured; // Texto informativo do botão que mostra a tecla, botão do mouse, joy... Que foi capturado

		UserInputEnum m_Type; // Serve como filtro de que tipo de entrada o botão aceita. ( Aqui não é a casa da mãe joana... )

		bool m_IsAwaiting; // Indica quando o botão está aguardando para fazer uma captura.
		int m_Code; // Código capturado.

		// Usado para evitar que mais de um botão entre no estado de espera de um key code.
		static UIPressButton* s_CurrentAwaiting;
	};
}

namespace core
{
	template<>
	struct EnumRegistry<editor::ui::UserInputEnum> {
		static constexpr std::array<EnumEntry, 3> entries = {{
			{ "Keyboard", static_cast<int>(editor::ui::UserInputEnum::Keyboard) },
			{ "MouseButton", static_cast<int>(editor::ui::UserInputEnum::MouseButton) },
			{ "JoyButton", static_cast<int>(editor::ui::UserInputEnum::JoyButton) }
		}};
	};

	static inline constexpr auto UserInputEnumDescriptor = 
		MakeEnumDescriptor<editor::ui::UserInputEnum>("UserInputEnum");
}