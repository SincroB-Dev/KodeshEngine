#include "manager.h"

#include "../../libs/imgui/backends/imgui_impl_sdl2.h"
#include "../../libs/imgui/backends/imgui_impl_opengl2.h"

namespace core
{
    namespace ui
    {
        const ImWchar UIManager::icons_ranges[3] = { 0xe000, 0xf8ff, 0 };
        
        ImFont* UIManager::MainFont = nullptr;
        ImFont* UIManager::IconifiedFont = nullptr;
        
        UIManager::UIManager(SDL_Window *window, SDL_GLContext context, SceneManager& sm)
            : sceneManager(sm), dockedToolbox(sm)
        {
            InitImGui(window, context);
            LogWindow::Instance = new LogWindow();
        }
        
        void UIManager::ProcessEvent(const SDL_Event *event)
        {
            ImGui_ImplSDL2_ProcessEvent(event);
        }
        
        // Aplica um tema parecido com o blender 3d, ainda está fixo, porém é temporario.
        void UIManager::SetTheme(ImGuiStyle& style, ImVec4* colors)
        {
            style.WindowRounding = 5.0f;
            style.FrameRounding = 3.0f;
            style.GrabRounding = 2.0f;
            style.ScrollbarRounding = 3.0f;
            style.FrameBorderSize = 0.0f;
            style.WindowBorderSize = 0.0f;
        
            colors[ImGuiCol_Text]                   = ImVec4(0.95f, 0.96f, 0.98f, 1.00f);
            colors[ImGuiCol_TextDisabled]           = ImVec4(0.36f, 0.42f, 0.47f, 1.00f);
            colors[ImGuiCol_WindowBg]               = ImVec4(0.13f, 0.14f, 0.17f, 1.00f);
            colors[ImGuiCol_ChildBg]                = ImVec4(0.13f, 0.14f, 0.17f, 1.00f);
            colors[ImGuiCol_PopupBg]                = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
            colors[ImGuiCol_Border]                 = ImVec4(0.25f, 0.25f, 0.25f, 0.32f);
            colors[ImGuiCol_FrameBg]                = ImVec4(0.20f, 0.21f, 0.24f, 1.00f);
            colors[ImGuiCol_FrameBgHovered]         = ImVec4(0.28f, 0.29f, 0.33f, 1.00f);
            colors[ImGuiCol_FrameBgActive]          = ImVec4(0.32f, 0.33f, 0.38f, 1.00f);
            colors[ImGuiCol_TitleBg]                = ImVec4(0.10f, 0.10f, 0.12f, 1.00f);
            colors[ImGuiCol_TitleBgActive]          = ImVec4(0.18f, 0.18f, 0.22f, 1.00f);
            colors[ImGuiCol_TitleBgCollapsed]       = ImVec4(0.10f, 0.10f, 0.12f, 0.75f);
            colors[ImGuiCol_MenuBarBg]              = ImVec4(0.14f, 0.14f, 0.17f, 1.00f);
            colors[ImGuiCol_ScrollbarBg]            = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
            colors[ImGuiCol_ScrollbarGrab]          = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
            colors[ImGuiCol_ScrollbarGrabHovered]   = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
            colors[ImGuiCol_ScrollbarGrabActive]    = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
            colors[ImGuiCol_CheckMark]              = ImVec4(0.26f, 0.59f, 0.98f, 1.00f); // azul/ciano
            colors[ImGuiCol_SliderGrab]             = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
            colors[ImGuiCol_SliderGrabActive]       = ImVec4(0.37f, 0.71f, 1.00f, 1.00f);
            colors[ImGuiCol_Button]                 = ImVec4(0.20f, 0.22f, 0.27f, 1.00f);
            colors[ImGuiCol_ButtonHovered]          = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
            colors[ImGuiCol_ButtonActive]           = ImVec4(0.06f, 0.53f, 0.98f, 1.00f);
            colors[ImGuiCol_Header]                 = ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
            colors[ImGuiCol_HeaderHovered]          = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
            colors[ImGuiCol_HeaderActive]           = ImVec4(0.06f, 0.53f, 0.98f, 1.00f);
            colors[ImGuiCol_Separator]              = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
            colors[ImGuiCol_SeparatorHovered]       = ImVec4(0.10f, 0.40f, 0.75f, 0.78f);
            colors[ImGuiCol_SeparatorActive]        = ImVec4(0.10f, 0.40f, 0.75f, 1.00f);
            colors[ImGuiCol_ResizeGrip]             = ImVec4(0.26f, 0.59f, 0.98f, 0.20f);
            colors[ImGuiCol_ResizeGripHovered]      = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
            colors[ImGuiCol_ResizeGripActive]       = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
            colors[ImGuiCol_Tab]                    = ImVec4(0.18f, 0.35f, 0.58f, 0.86f);
            colors[ImGuiCol_TabHovered]             = ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
            colors[ImGuiCol_TabActive]              = ImVec4(0.20f, 0.41f, 0.68f, 1.00f);
            colors[ImGuiCol_TabUnfocused]           = ImVec4(0.07f, 0.10f, 0.15f, 0.97f);
            colors[ImGuiCol_TabUnfocusedActive]     = ImVec4(0.14f, 0.26f, 0.42f, 1.00f);
        }
        
        // Inicialização do Dear ImGui
        void UIManager::InitImGui(SDL_Window *window, SDL_GLContext context)
        {
            // IMGUI Init
            IMGUI_CHECKVERSION();
            ImGui::CreateContext();
            ImGuiIO &io = ImGui::GetIO();
            
            LoadFonts(io);
        
            ImGui::StyleColorsDark();
            ImGuiStyle &style = ImGui::GetStyle();
            ImVec4 *colors = style.Colors;
        
            SetTheme(style, colors);
        
            ImGui_ImplSDL2_InitForOpenGL(window, context);
            ImGui_ImplOpenGL2_Init();
        }
        
        // Carregamento das fontes e icones da interface
        void UIManager::LoadFonts(ImGuiIO &io)
        {
            icons_config.MergeMode = true;
            icons_config.PixelSnapH = true;
        
            io.Fonts->Clear();
        
            MainFont = io.Fonts->AddFontFromFileTTF("../assets/fonts/segoeui.ttf", 14.0f);
            IconifiedFont = io.Fonts->AddFontFromFileTTF("../assets/fonts/MaterialIcons-Regular.ttf", 16.0f, &icons_config, UIManager::icons_ranges);
        }
        
        // Menu principal da apĺicação
        void UIManager::MainMenu()
        {
            if (ImGui::BeginMainMenuBar())
            {
                if (ImGui::BeginMenu("Arquivo"))
                {
                    if (ImGui::BeginMenu("Projeto"))
                    {
                        if (ImGui::MenuItem("Novo Projeto", "Ctrl+N")) {
                            // ação de novo
                        }
                        if (ImGui::MenuItem("Abrir Projeto", "Ctrl+O")) {
                            // ação de abrir
                        }
                        if (ImGui::MenuItem("Salvar Projeto", "Ctrl+S")) {
                            // ação de salvar
                        }
                        
                        ImGui::EndMenu();
                    }
                    
                    if (ImGui::BeginMenu("Importar"))
                    {
                        if (ImGui::MenuItem("Script")) {
                            // ação de novo
                        }
                        if (ImGui::MenuItem("Imagem")) {
                            // ação de abrir
                        }
                        
                        ImGui::EndMenu();
                    }
                    
                    ImGui::Separator();
                    if (ImGui::MenuItem("Sair", "Alt+F4")) {
                        // ação de sair
                    }
                    
                    ImGui::EndMenu();
                }

                if (ImGui::BeginMenu("Editar"))
                {
                    if (ImGui::BeginMenu("Adicionar"))
                    {
                        if (ImGui::MenuItem("Entity")) {
                            // ação
                        }
                        ImGui::Separator();
                        
                        if (ImGui::MenuItem("Retangulo")) {
                            // ação
                        }

                        
                        if (ImGui::MenuItem("Circulo")) {
                            // ação
                        }
                        
                        
                        if (ImGui::MenuItem("Estrela")) {
                            // ação
                        }
                        
                        ImGui::EndMenu();
                    }
                    ImGui::EndMenu();
                }
                
                if (!LogWindow::Instance->isOpen || !luaTextEditor.isOpen)
                {
                    if (ImGui::BeginMenu("Workspace"))
                    {
                        if (!luaTextEditor.isOpen)
                        {
                            if (ImGui::MenuItem("Editor de Scripts")) {
                                luaTextEditor.isOpen = true;
                            }
                        }
                        if (!LogWindow::Instance->isOpen)
                        {
                            if (ImGui::MenuItem("Logs")) {
                                LogWindow::Instance->isOpen = true;
                            }
                        }

                        ImGui::EndMenu();
                    }
                }

                ImGui::EndMainMenuBar();
            }

            // depois do EndMainMenuBar
            menuHeight = ImGui::GetFrameHeight();
        }
        
        // Renderização da interface
        void UIManager::Render(SceneManager& scene)
        {
            ImGui_ImplOpenGL2_NewFrame();
            ImGui_ImplSDL2_NewFrame();
            ImGui::NewFrame();

            // Menu principal no topo
            MainMenu();

            // Painéis dockáveis ou flutuantes
            dockedToolbox.Draw(menuHeight); // Janela dock fixa a direita
            LogWindow::Instance->Show();
            luaTextEditor.Render();

            // Renderiza a UI
            ImGui::Render();
            ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
        }

    };
};
