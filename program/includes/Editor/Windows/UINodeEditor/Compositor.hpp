#pragma once

#include "Editor/Windows/UINodeEditor/Node.hpp"
#include "Editor/Windows/UINodeEditor.hpp"
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
		static Node* BaseNode(UINodeEditor* editor, std::string name);

		//-----------------------------
		// Inputs básicos
		//-----------------------------

		/**
		 * @brief Node que captura eventos de teclado e os entrega como saída.
		 **/
		static Node* KeyboardEventNode(UINodeEditor* editor);

		/**
		 * @brief Node que captura eventos do mouse e os entrega como saída.
		 **/
		static Node* MouseEventNode(UINodeEditor* editor);

		/**
		 * @brief Node que sempre vai entregar uma saída verdadeira, ativando sempre aquele conectado. 
		 **/
		static Node* AlwaysEventNode(UINodeEditor* editor);

		//-------------------------------
		// Captura da cena
		//-------------------------------

		/**
		 * @brief Captura uma entidade da cena por Tag, e entrega seu ID como saída.
		 **/
		static Node* GetEntityNode(UINodeEditor* editor);

		//-------------------------------
		// Saída de dados
		//-------------------------------

		/**
		 * @brief Recebe qualquer saída e a converte em um texto que pode ser lido no node.
		 *        finalidade de testar a saída de valores e o recebimento dos mesmos.
		 **/
		static Node* TestOutputNode(UINodeEditor* editor);
	};
}