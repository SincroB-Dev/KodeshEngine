#pragma once

#include <string>

namespace core::utils
{
	struct Version
	{
		int Major;
		int Minor;
		int Patch;

    	std::string ToString() const 
    	{
	        return std::to_string(Major) + "." + 
	               std::to_string(Minor) + "." + 
	               std::to_string(Patch);
	    }
	};

	/**
	 * @brief Primeira citação da versão, e o que funciona nessa versão?
	 * 
	 *  - ECS (Entity Component System)
	 *  - System Manangement (Sistemas acopláveis, facilitando integrações, novas funcionalidades...)
	 *    + SceneManager sendo o principal gerenciador de scenes atualmente
	 *    + UILayerManager sendo o principal gerenciador de interface de usuário
	 *  - Renderer completamente desacoplado (Permitindo a integração de novo renderizador de forma facilitada)
	 *  - Gerenciador de eventos (Totalmente integrada a gerenciamento de eventos, deixando mais fácil envio de comandos entre componentes e sistemas)
	 * 	- Sistema de Logs (Totalmente desacoplado da interface, o que deixa bem mais limpo, caso a interface não exista o log é lançado no terminal)
	 * 
	 * Utilizando na interface de usuário (Dear ImGui), biblioteca para gerenciamento de janelas (SDL2).
	 * 
	 * Interfaces de usuário prontas/em construção:
	 *  
	 * 	- UIMenuBar: Responsável por construir menu de ferramentas. (em construção)
	 * 
	 *  - UILogger: Responsável por apresentar os logs contidos em LogManager. (em testes)
	 *  - UINodeEditor: Responsável por apresentar uma condição visual dos nodes. (em construção)
	 * 
	 * Modos de Engine:
	 * 
	 * 	- Editor: Neste modo a engine é preparada para o estado de edição do jogo.
	 *  - Play: Modo de testes, onde o cenário do jogo é executado.
	 **/
	inline const Version Current{0,0,8};
}