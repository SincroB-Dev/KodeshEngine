#pragma once

namespace editor::nodes
{
	/**
	 * @brief Especifica a natureza do socket, entrada ou saída de informações.
	 **/
	enum class SocketKind
	{
		Input,
		Output
	};

	/**
	 * @brief Indica o tipo de nó dentro do editor, cada nó tem uma finalidade diferente.
	 **/
	enum class NodeType: uint32_t
	{
		None      = 0,
		Blueprint = 1 << 0, // Nó lógico principal
		Simple    = 1 << 1, // Nó basico para operações diretas
		Tree      = 1 << 2, // Nó hierarquico (Para decisões em arvore)
		Comment   = 1 << 3, // Para organização visual
		Houdini   = 1 << 4, // Nó especial de integração externa

		InputEvent = 1 << 8, // Nó que trabalha com entrada de inputs de usuário (teclado, mouse, gamepad)
		Action     = 1 << 10, // Indica que é um nó de ação, ele tem seu Evaluate chamado no OnUpdate da engine
	};

	/**
	 * @brief Bitwise OR inclusivo.
	 **/
	inline NodeType operator | (NodeType a, NodeType b)
	{
		return static_cast<NodeType>(
			static_cast<uint32_t>(a) | static_cast<uint32_t>(b)
		);
	} 

	/**
	 * @brief Bitwise AND comparativo
	 **/
	inline NodeType operator & (NodeType a, NodeType b)
	{
		return static_cast<NodeType>(
			static_cast<uint32_t>(a) & static_cast<uint32_t>(b)
		);
	}
}