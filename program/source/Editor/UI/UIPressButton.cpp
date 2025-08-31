#include "Editor/UI/UIPressButton.hpp"
#include "Editor/UI/UITooltip.hpp"

#include <imgui/imgui.h>

namespace editor::ui
{
	// Inicialização do static que verifica se tem 1 botão esperando input
	UIPressButton* UIPressButton::s_CurrentAwaiting = nullptr;

	void UIPressButton::HandlePressButton()
	{
		if (m_IsAwaiting)
		{
			for (int key = ImGuiKey_NamedKey_BEGIN; key < ImGuiKey_NamedKey_END; key++)
			{
			    ImGuiKey imguiKey = (ImGuiKey)key;

			    if (ImGui::IsKeyPressed(imguiKey))
			    {
			    	if ((m_Type == UserInputEnum::Keyboard && key >= ImGuiKey_MouseLeft) ||
			    		(m_Type == UserInputEnum::MouseButton && key < ImGuiKey_MouseLeft))
			    	{
			    		SetCaptured(-1);
			    		break;
			    	}

			        SetCaptured(key);
                    break;
			    }
			}
		}
	}

	void UIPressButton::SetCaptured(int code)
	{
        m_Captured = code > 0 ? ImGui::GetKeyName((ImGuiKey)code) : "<Empty>";
        
        m_Code = code;
        m_IsAwaiting = false;

        s_CurrentAwaiting = nullptr;
	}

	void UIPressButton::ResetButtonState()
	{
		SetCaptured(-1);
	}

	void UIPressButton::Render()
	{
		std::string label = m_IsAwaiting
							? m_AwaitingLabel
							: m_Code > 0
								? m_Captured
								: m_EmptyLabel;

		bool isPushStyleColor = false;

		// Caso o botão esteja aguardando algum evento, ele é travado na cor de active
		if (m_IsAwaiting)
		{
			ImVec4 newBtnColor = ImGui::GetStyleColorVec4(ImGuiCol_ButtonActive);

			ImGui::PushStyleColor(ImGuiCol_Button, newBtnColor);
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, newBtnColor);
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, newBtnColor);

			isPushStyleColor = true;
		}

		// Identificador único do botão, evitando o uso do UniqueID para componentes de interface
		ImGui::PushID(this);
		{
			float width = ImGui::CalcItemWidth(); // pega o que PushItemWidth definir, ou tamanho default
    		width = (width < 80) ? 80 : width;

			if (ImGui::Button(label.c_str(), ImVec2(width, 25)) && !m_IsAwaiting)
			{
				if (s_CurrentAwaiting == nullptr)
				{
					m_IsAwaiting = true;
					s_CurrentAwaiting = this;
				}
				else
				{
					if (this != s_CurrentAwaiting)
					{
						ResetButtonState();
					}
				}
			}

			if (ImGui::IsItemHovered())
			{
				UITooltip::SetTooltip("Armazena entrada de eventos (teclado/mouse/gamepad) de\nusuário para uso posterior (in-game)");
			}
		}
		ImGui::PopID();

		// Remove o estilo do botão, solução per-frame
		if (isPushStyleColor)
		{
			ImGui::PopStyleColor(3);
		}

		HandlePressButton();
	}
}