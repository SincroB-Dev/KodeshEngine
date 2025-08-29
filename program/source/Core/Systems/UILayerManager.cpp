#include "Core/Systems/UILayerManager.hpp"

#include "Core/Events/TextInputEvent.hpp"
#include "Core/Events/KeyboardEvent.hpp"
#include "Core/Events/MouseEvent.hpp"
#include "Core/Helpers/LogManager.hpp"

#include <imgui/backends/imgui_impl_sdl2.h>
#include <imgui/backends/imgui_impl_opengl2.h>

#include "Editor/UI/Fontes/MaterialIcons.hpp"
#include "Core/Application/KodeshApplication.hpp"
#include "Editor/Windows/UINodeEditor.hpp"

#include <nlohmann/json.hpp>
#include <fstream>

using json = nlohmann::json;

using namespace editor;
using namespace editor::ui;

namespace core::systems
{
    /**
     * @todo this-> pode gerar dangling references caso UI seja destruída, migrar para weak->ptr.
     **/
	UILayerManager::UILayerManager(events::EventDispatcher& dispatcher, input::InputManager& input, std::string configsPath)
		: m_ImGuiInitialized(false), m_ConfigsPath(configsPath)
	{
        // Registra os callbacks que vão para a interface, lembrando que ao registrar esses eventos
        // no dispatcher, ele vai dar prioridade para aqueles que foram inseridos primeiro, então,
        // o layout deve ser o primeiro a registrar seus eventos.

        // text input event
        dispatcher.Register<events::TextInputEvent>([this](events::Event& e){
            this->OnEvent(e);
        });

        // key events
        dispatcher.Register<events::KeyPressedEvent>([this](events::Event& e) {
            this->OnEvent(e);
        });

        dispatcher.Register<events::KeyReleasedEvent>([this](events::Event& e) {
            this->OnEvent(e);
        });

        // mouse events
        dispatcher.Register<events::MouseButtonPressedEvent>([this](events::Event& e) {
            this->OnEvent(e);
        });

        dispatcher.Register<events::MouseButtonReleasedEvent>([this](events::Event& e) {
            this->OnEvent(e);
        });

        dispatcher.Register<events::MouseMovedEvent>([this](events::Event& e) {
            this->OnEvent(e);
        });

        dispatcher.Register<events::MouseScrolledEvent>([this](events::Event& e) {
            this->OnEvent(e);
        });
	}

	void UILayerManager::OnEvent(events::Event& e)
	{
        // Processa os eventos da UI
        ImGui_ImplSDL2_ProcessEvent((SDL_Event*)e.NativeEvent);

        ImGuiIO &io = ImGui::GetIO();

        // Consome os eventos para que proximos itens não os use.
        if (io.WantCaptureMouse || io.WantCaptureKeyboard)
        {
			e.Handled = true; // Impede que a cena receba o evento.
        }
	}

    void UILayerManager::InitImGui(SDL_Window* window, SDL_GLContext* context)
    {
    	if (m_ImGuiInitialized)
    	{
    		return;
    	}

        // IMGUI Init
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();

        ImGuiIO &io = ImGui::GetIO();
    
        ImGuiStyle &style = ImGui::GetStyle();
        ImVec4 *colors = style.Colors;
    
        WriteDefaultTheme(m_ConfigsPath);

        LoadTheme(m_ConfigsPath, style, colors);
        LoadFonts(m_ConfigsPath, io);

        LoadMaterialIconsFont(io);
    
        ImGui_ImplSDL2_InitForOpenGL(window, *context);
        ImGui_ImplOpenGL2_Init();

        m_ImGuiInitialized = true;
    }

    void UILayerManager::RenderTopMenu()
    {
        if (ImGui::BeginMainMenuBar())
        {
            for (editor::ui::UIMenuComponent& menu : m_LayoutMenubar)
            {
                menu.Render();
            }

            ImGui::EndMainMenuBar();
        }
        // depois do EndMainMenuBar
        ImGui::GetFrameHeight();
    }

    /*unused*/
	void UILayerManager::Update(double deltaTime){}

	void UILayerManager::Render(renderer::Renderer& renderer, int w, int h, double deltaTime)
	{
        ImGui_ImplOpenGL2_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();

		// Renderiza ImGui ou outra UI
        RenderTopMenu();

        // Renderiza as janelas;
        for (auto& win : m_Windows)
            win.second->OnImGuiRender();

        // Renderiza a UI
        ImGui::Render();
        ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
	}

    UIMenuComponent& UILayerManager::AddLayoutMenubarItem(const std::string& Label)
    {
        return m_LayoutMenubar.emplace_back(
            UIMenuComponent(Label, "")
        );
    }

    void UILayerManager::AddLayoutMenubarSeparator(editor::ui::UIMenuComponent& Menu)
    {
        Menu.m_Childrens.emplace_back(
            std::make_unique<editor::ui::UISeparator>()
        );
    }

    int UILayerManager::AddWindow(std::unique_ptr<windows::UIWindow> window)
    {
        utils::UniqueID windowId = m_UniqueLayoutGen.CreateUniqueID();
        m_Windows[windowId.ID] = std::move(window);
        return windowId.ID;
    }

    windows::UIWindow* UILayerManager::GetWindow(int id)
    {
        return m_Windows[id].get();
    }

    void UILayerManager::WriteDefaultTheme(const std::string& path)
    {
        json configs = app::KodeshApplication::GetConfigs();

        // Caso o tema já esteja listado nas configurações.
        if (configs.contains("theme")) return;

        configs["theme"]= { 
            { "based_on", "dark" },
            { "style", {
                { "WindowRounding", 5.0 },
                { "FrameRounding", 3.0 },
                { "GrabRounding", 2.0 },
                { "ScrollbarRounding", 3.0 },
                { "FrameBorderSize", 0.0 },
                { "WindowBorderSize", 0.0 }
            }},
            { "colors", {
                { "Text", {0.95, 0.96, 0.98, 1.0} },
                { "TextDisabled", {0.36, 0.42, 0.47, 1.0} },
                { "WindowBg", {0.13, 0.14, 0.17, 1.0} },
                { "ChildBg", {0.13, 0.14, 0.17, 1.0} },
                { "PopupBg", {0.08, 0.08, 0.08, 0.94} },
                { "Border", {0.25, 0.25, 0.25, 0.32} },
                { "FrameBg", {0.20, 0.21, 0.24, 1.0} },
                { "FrameBgHovered", {0.28, 0.29, 0.33, 1.0} },
                { "FrameBgActive", {0.32, 0.33, 0.38, 1.0} },
                { "TitleBg", {0.10, 0.10, 0.12, 1.0} },
                { "TitleBgActive", {0.18, 0.18, 0.22, 1.0} },
                { "TitleBgCollapsed", {0.10, 0.10, 0.12, 0.75} },
                { "MenuBarBg", {0.14, 0.14, 0.17, 1.0} },
                { "ScrollbarBg", {0.02, 0.02, 0.02, 0.53} },
                { "ScrollbarGrab", {0.31, 0.31, 0.31, 1.0} },
                { "ScrollbarGrabHovered", {0.41, 0.41, 0.41, 1.0} },
                { "ScrollbarGrabActive", {0.51, 0.51, 0.51, 1.0} },
                { "CheckMark", {0.26, 0.59, 0.98, 1.0} },
                { "SliderGrab", {0.26, 0.59, 0.98, 1.0} },
                { "SliderGrabActive", {0.37, 0.71, 1.0, 1.0} },
                { "Button", {0.20, 0.22, 0.27, 1.0} },
                { "ButtonHovered", {0.26, 0.59, 0.98, 1.0} },
                { "ButtonActive", {0.06, 0.53, 0.98, 1.0} },
                { "Header", {0.26, 0.59, 0.98, 0.80} },
                { "HeaderHovered", {0.26, 0.59, 0.98, 1.0} },
                { "HeaderActive", {0.06, 0.53, 0.98, 1.0} },
                { "Separator", {0.43, 0.43, 0.50, 0.50} },
                { "SeparatorHovered", {0.10, 0.40, 0.75, 0.78} },
                { "SeparatorActive", {0.10, 0.40, 0.75, 1.0} },
                { "ResizeGrip", {0.26, 0.59, 0.98, 0.20} },
                { "ResizeGripHovered", {0.26, 0.59, 0.98, 0.67} },
                { "ResizeGripActive", {0.26, 0.59, 0.98, 0.95} },
                { "Tab", {0.18, 0.35, 0.58, 0.86} },
                { "TabHovered", {0.26, 0.59, 0.98, 0.80} },
                { "TabActive", {0.20, 0.41, 0.68, 1.0} },
                { "TabUnfocused", {0.07, 0.10, 0.15, 0.97} },
                { "TabUnfocusedActive", {0.14, 0.26, 0.42, 1.0} }
            }}
        };

        std::ofstream out(path);
        out << configs.dump(4);
    }

    void UILayerManager::LoadTheme(const std::string& path, ImGuiStyle& style, ImVec4* colors)
    {
        json configs = app::KodeshApplication::GetConfigs();

        // Sem verificações pois sempre na primeira carga o arquivo vai existir.
        json theme = configs["theme"];

        // aplica um tema base
        std::string base = theme.value("based_on", "dark");

        if (base == "dark") ImGui::StyleColorsDark();
        else if (base == "light") ImGui::StyleColorsLight();
        else if (base == "classic") ImGui::StyleColorsClassic();
        else LogManager::Log(LogType::Warning, "Atenção, chave 'based_on' deve ter um dos valores: dark, light, classic.");

        // aplica style
        style.WindowRounding   = theme["style"].value("WindowRounding", style.WindowRounding);
        style.FrameRounding    = theme["style"].value("FrameRounding", style.FrameRounding);
        style.GrabRounding     = theme["style"].value("GrabRounding", style.GrabRounding);
        style.ScrollbarRounding= theme["style"].value("ScrollbarRounding", style.ScrollbarRounding);
        style.FrameBorderSize  = theme["style"].value("FrameBorderSize", style.FrameBorderSize);
        style.WindowBorderSize = theme["style"].value("WindowBorderSize", style.WindowBorderSize);

        // aplica cores
        auto setColor = [&](ImGuiCol idx, const std::string& key) {
            if (theme["colors"].contains(key)) {
                auto c = theme["colors"][key];
                if (c.is_array() && c.size() == 4)
                {
                    colors[idx] = ImVec4(c[0], c[1], c[2], c[3]);
                    return;
                }
                LogManager::Log(LogType::Warning, "Chave de tema '", key, "' incompleta.");
                return;
            }
            std::cout << "Atenção! Chave de tema '" << key << "' não encontrada." << std::endl;
        };

        setColor(ImGuiCol_Text, "Text");
        setColor(ImGuiCol_TextDisabled, "TextDisabled");
        setColor(ImGuiCol_WindowBg, "WindowBg");
        setColor(ImGuiCol_ChildBg, "ChildBg");
        setColor(ImGuiCol_PopupBg, "PopupBg");
        setColor(ImGuiCol_Border, "Border");
        setColor(ImGuiCol_FrameBg, "FrameBg");
        setColor(ImGuiCol_FrameBgHovered, "FrameBgHovered");
        setColor(ImGuiCol_FrameBgActive, "FrameBgActive");
        setColor(ImGuiCol_TitleBg, "TitleBg");
        setColor(ImGuiCol_TitleBgActive, "TitleBgActive");
        setColor(ImGuiCol_TitleBgCollapsed, "TitleBgCollapsed");
        setColor(ImGuiCol_MenuBarBg, "MenuBarBg");
        setColor(ImGuiCol_ScrollbarBg, "ScrollbarBg");
        setColor(ImGuiCol_ScrollbarGrab, "ScrollbarGrab");
        setColor(ImGuiCol_ScrollbarGrabHovered, "ScrollbarGrabHovered");
        setColor(ImGuiCol_ScrollbarGrabActive, "ScrollbarGrabActive");
        setColor(ImGuiCol_CheckMark, "CheckMark");
        setColor(ImGuiCol_SliderGrab, "SliderGrab");
        setColor(ImGuiCol_SliderGrabActive, "SliderGrabActive");
        setColor(ImGuiCol_Button, "Button");
        setColor(ImGuiCol_ButtonHovered, "ButtonHovered");
        setColor(ImGuiCol_ButtonActive, "ButtonActive");
        setColor(ImGuiCol_Header, "Header");
        setColor(ImGuiCol_HeaderHovered, "HeaderHovered");
        setColor(ImGuiCol_HeaderActive, "HeaderActive");
        setColor(ImGuiCol_Separator, "Separator");
        setColor(ImGuiCol_SeparatorHovered, "SeparatorHovered");
        setColor(ImGuiCol_SeparatorActive, "SeparatorActive");
        setColor(ImGuiCol_ResizeGrip, "ResizeGrip");
        setColor(ImGuiCol_ResizeGripHovered, "ResizeGripHovered");
        setColor(ImGuiCol_ResizeGripActive, "ResizeGripActive");
        setColor(ImGuiCol_Tab, "Tab");
        setColor(ImGuiCol_TabHovered, "TabHovered");
        setColor(ImGuiCol_TabActive, "TabActive");
        setColor(ImGuiCol_TabUnfocused, "TabUnfocused");
        setColor(ImGuiCol_TabUnfocusedActive, "TabUnfocusedActive");
    }

    void UILayerManager::LoadFonts(const std::string& path, ImGuiIO& io) 
    {
        json configs = app::KodeshApplication::GetConfigs();

        // Caso as fontes não estejam listadas nas configurações, ele só ignora.
        if (!configs.contains("fonts")) return;

        // Limpeza de fontes do imgui, para inserção de fonte personalizada
        io.Fonts->Clear();

        // Percorre pelas fontes descritas nas configurações para a inserção na UI.
        for (auto& font : configs["fonts"])
        {
            ImFontConfig cfg;

            if (font.contains("extras")) {
                auto& c = font["extras"];

                if (c.value("MergeMode", false)) cfg.MergeMode = true;
                if (c.value("PixelSnapH", false)) cfg.PixelSnapH  = true;
            }

            ImFont* fontPtr = nullptr;

            fontPtr = io.Fonts->AddFontFromFileTTF(
                font["path"].get<std::string>().c_str(),
                font["size"].get<float>(), 
                &cfg
            );

            if (fontPtr) {
                m_Fonts[font["name"]] = fontPtr; // guarda no mapa

            } else {
                std::cerr << "Falha ao carregar fonte: " << font["path"] << "\n";
            }
        }
    }

    void UILayerManager::LoadMaterialIconsFont(ImGuiIO& io)
    {
        ImFontConfig cfg;

        cfg.MergeMode = true;
        cfg.PixelSnapH = true;

        m_Fonts["MaterialIcons"] = io.Fonts->AddFontFromMemoryTTF(
            MaterialIcons_Regular_ttf, 
            MaterialIcons_Regular_ttf_len, 
            16.0f, &cfg, 
            MaterialIcons_Regular_ranges
        );
    }
}