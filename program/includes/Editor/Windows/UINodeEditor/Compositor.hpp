#pragma once

#include "Editor/Windows/UINodeEditor/Nodes/Node.hpp"
#include "Editor/Windows/UINodeEditor.hpp"

#include "Core/Application/KodeshApplication.hpp"

#include <string>

namespace editor::nodes::compositor
{
	/**
	 * @brief Essa classe é um conjunto de métodos, não pode ser instanciada,
	 *        ela foi construída apenas para o agrupamento de métodos de construção
	 *        de nodes padrão da Kodesh.
	 **/
	class Compositor
	{
	public:
		Compositor() = delete;

		/**
		 * @brief Cria um node básico, ele já faz entradas no lookup do editor.
		 **/
		static void AddSocketsCallbacks(UINodeEditor* editor, Node* node);

		//-----------------------------
		// Inputs básicos
		//-----------------------------

		/**
		 * @brief Node que captura eventos de teclado e os entrega como saída.
		 **/
		static Node* InputEventNode(core::app::KodeshApplication& app, UINodeEditor* editor);

		/**
		 * @brief Node que sempre vai entregar uma saída verdadeira, ativando sempre aquele conectado. 
		 **/
		static Node* OnUpdateNode(core::app::KodeshApplication& app, UINodeEditor* editor);
	};
}