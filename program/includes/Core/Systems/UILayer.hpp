#pragma once

// Core
#include "Core/Events/EventDispatcher.hpp"
#include "Core/Events/Event.hpp"
#include "Core/Input/InputManager.hpp"
#include "Core/Systems/ISystem.hpp"
#include "Core/Renderer/Renderer.hpp"
#include "Core/Utils/UniqueIDGen.hpp"

// Editor UI
#include "Editor/UI/UIMenuComponent.hpp"
#include "Editor/UI/UISeparator.hpp"

#include "Editor/Windows/UIWindow.hpp"

// Third-party
#include <imgui/imgui.h>
#include <SDL2/SDL.h>

// STL
#include <vector>
#include <string>

namespace core::systems
{
    /**
     * @class UILayer
     * @brief Sistema responsável pela camada de interface gráfica (ImGui).
     *
     * - Captura eventos primeiro (antes da cena/game).
     * - Gerencia inicialização do ImGui com SDL/OpenGL.
     * - Renderiza menus e layouts customizados.
     * - Oferece API para criação dinâmica de menus.
     */
    class UILayer : public ISystem
    {
    public:
        /**
         * @brief Constrói a camada de UI.
         */
        UILayer(events::EventDispatcher& dispatcher, input::InputManager& input, std::string configsPath);

        ~UILayer() 
        {
        	std::cout << "[UI] Limpando mapa de fontes..." << std::endl;
        	m_Fonts.clear();
        }

        /**
         * @brief Recebe e trata eventos do sistema.
         */
        void OnEvent(events::Event& e) override;
        
        /**
         * @brief Atualiza a UI (lógica não-renderizável).
         */
        void Update(double deltaTime) override;

        /**
         * @brief Renderiza a UI com ImGui.
         */
        void Render(renderer::Renderer& renderer, int w, int h, double deltaTime) override;

        /**
         * @brief Inicializa o ImGui com suporte ao SDL2/OpenGL.
         */
        void InitImGui(SDL_Window* window, SDL_GLContext* context);

        /// Renderiza a barra superior de menus padrão.
        void RenderTopMenu();

        /**
         * @brief Escreve o tema visual padrão da engine em um arquivo. 
         **/
        void WriteDefaultTheme(const std::string& path);

        /**
         * @brief Carrega o tema visual da engine.
         **/
        void LoadTheme(const std::string& path, ImGuiStyle& style, ImVec4* colors);

        /**
         * @brief Carrega as fontes que forem especificadas no arquivo de configurações.
         **/
        void LoadFonts(const std::string& path, ImGuiIO& io);

        /**
         * @brief Carrega a fonte de icones do MaterialIcons-Regular, embarcada no software. 
         **/
        void LoadMaterialIconsFont(ImGuiIO& io);

        /**
         * @brief Adiciona item diretamente no menubar principal.
         */
        editor::ui::UIMenuComponent& AddLayoutMenubarItem(const std::string& label);

        /**
         * @brief Adiciona um separador visual no menu.
         */
        void AddLayoutMenubarSeparator(editor::ui::UIMenuComponent& menu);

        /**
         * @brief Adiciona item dentro de um menu específico (hierarquia).
         */
        template<typename... Funcs>
        editor::ui::UIMenuComponent& AddLayoutMenubarItem(
            editor::ui::UIMenuComponent& menu,
            const std::string& label,
            const std::string& shortcut,
            Funcs&&... callbacks
        );

        /**
         * @brief Adiciona uma janela para ser renderizada.
         **/
        int AddWindow(std::unique_ptr<editor::windows::UIWindow> window);

        /**
         * @brief Captura uma janela pela sua identificação.
         **/
        editor::windows::UIWindow* GetWindow(int id);

    private:
        bool m_ImGuiInitialized = false; // Marca se o ImGui já foi inicializado.

        utils::UniqueIDGen m_UniqueLayoutGen; // Gerador de IDs únicos janelas no layout.
        
        std::vector<editor::ui::UIMenuComponent> m_LayoutMenubar; // Lista principal de menus.
    	std::string m_ConfigsPath; // Path do arquivo de configurações da engine.

    	// Neste caso não uso unique_ptr, pois quem cuida de liberar a memória é o ImGui
    	std::unordered_map<std::string, ImFont*> m_Fonts; // Fonts carregadas no layout.
        std::unordered_map<int, std::unique_ptr<editor::windows::UIWindow>> m_Windows; // Janelas do sistema, separadas por unordered para permitir pegar janelas por id.
    };

    // ------------------------------
    // Template implementation
    // ------------------------------
    template<typename... Funcs>
    editor::ui::UIMenuComponent& UILayer::AddLayoutMenubarItem(
        editor::ui::UIMenuComponent& menu,
        const std::string& label,
        const std::string& shortcut,
        Funcs&&... callbacks
    )
    {
        menu.m_Childrens.push_back(
            std::make_unique<editor::ui::UIMenuComponent>(
                label,
                shortcut,
                std::forward<Funcs>(callbacks)...
            )
        );

        return *static_cast<editor::ui::UIMenuComponent*>(menu.m_Childrens.back().get());
    }
}
