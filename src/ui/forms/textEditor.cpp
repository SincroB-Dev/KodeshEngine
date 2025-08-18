#include "textEditor.h"

#include "../../../libs/imgui/imgui.h"
#include "../../../libs/ImGuiFileDialog/ImGuiFileDialog.h"

namespace core
{
    namespace ui
    {

        LuaTextEditor::LuaTextEditor(): isOpen(false)
        {
        }
        
        LuaTextEditor::~LuaTextEditor()
        {
        }
        
        /**
         * Tema inspirado no blender 3d
         */
        const TextEditor::Palette& LuaTextEditor::GetDefaultTheme()
        {
            const static TextEditor::Palette p = { {
                0xfff0f0f0, // Default (texto normal, cinza claro)
                0xffffcc66, // Keyword (amarelo suave)
                0xff66ccff, // Number (azul suave)
                0xffa0ffa0, // String (verde claro)
                0xffa0ffa0, // Char literal
                0xfff0f0f0, // Punctuation
                0xffcccccc, // Preprocessor (#include, #define)
                0xfff0f0f0, // Identifier
                0xfff0f0f0, // Known identifier
                0xffff99cc, // Preproc identifier
                0xff888888, // Comment (single line, cinza médio)
                0xff888888, // Comment (multi line)
                0xff222222, // Background (cinza escuro neutro)
                0xffffffff, // Cursor (branco)
                0x80408080, // Selection (translúcido, cinza)
                0x80ff4040, // ErrorMarker (vermelho suave)
                0x4080ff00, // Breakpoint (verde translúcido)
                0xff707070, // Line number (cinza médio)
                0x40202020, // Current line fill (translúcido)
                0x40404040, // Current line fill (inactive)
                0x40808080, // Current line edge
            } };
            return p;
        }

        void LuaTextEditor::AddTab(std::string name, const std::string& content, const std::string& path) {
            // 1. Garantir extensão .lua
            if (name.size() < 4 || name.substr(name.size() - 4) != ".lua") {
                name += ".lua";
            }

            // 2. Verificar duplicatas e gerar nome único
            std::string baseName = name.substr(0, name.size() - 4); // sem extensão
            std::string extension = ".lua";

            int duplicateCount = 0;
            bool nameExists = true;

            while (nameExists) {
                nameExists = false;
                for (const auto& tab : tabs) {
                    if (tab.name == name) {
                        duplicateCount++;
                        name = baseName + "_" + std::to_string(duplicateCount) + extension;
                        nameExists = true;
                        break;
                    }
                }
            }

            // 3. Criar nova aba
            tabs.push_back({ name, path, content, true, false, false });
            activeTab = (int)tabs.size() - 1;
            mainEditor.SetText(content);
        }


        void LuaTextEditor::CloseTab(int index) {
            if (index >= 0 && index < (int)tabs.size()) {
                tabs.erase(tabs.begin() + index);
                if (tabs.empty()) {
                    activeTab = -1;
                    mainEditor.SetReadOnly(true);
                    mainEditor.SetText("");
                } else {
                    activeTab = std::min(index, (int)tabs.size() - 1);
                    mainEditor.SetText(tabs[activeTab].content);
                }
            }
        }

        void LuaTextEditor::HandleOpenDialog() {
            if (ImGuiFileDialog::Instance()->Display("OpenFileDlg", ImGuiWindowFlags_None)) {
                if (ImGuiFileDialog::Instance()->IsOk()) {
                    std::string filePath = ImGuiFileDialog::Instance()->GetFilePathName();
                    std::string fileName = ImGuiFileDialog::Instance()->GetCurrentFileName();

                    // Lê o conteúdo
                    std::ifstream inFile(filePath);
                    std::string content((std::istreambuf_iterator<char>(inFile)), std::istreambuf_iterator<char>());
                    inFile.close();

                    // Cria ou reutiliza aba
                    AddTab(fileName, content);
                    tabs[activeTab].path = filePath;
                    tabs[activeTab].isDirty = false;   // recém aberto
                }
                ImGuiFileDialog::Instance()->Close();
            }
        }

        void LuaTextEditor::SaveFile() {
            if (activeTab < 0) return;
            auto& t = tabs[activeTab];

            std::string path = t.path;
            if (path.empty()) {
                // se não tiver caminho definido, abrir SaveDialog
                IGFD::FileDialogConfig cfg;
                cfg.path = ".";
                cfg.flags |= ImGuiFileDialogFlags_::ImGuiFileDialogFlags_Modal;
                
                ImGuiFileDialog::Instance()->OpenDialog("SaveFileDlg", "Salvar Lua", ".lua", cfg);
                return;
            }

            // Atualiza conteúdo e salva
            t.content = mainEditor.GetText();
            std::ofstream outFile(path);
            outFile << t.content;
            outFile.close();
            t.isDirty = false;
        }

        void LuaTextEditor::HandleSaveDialog() {
            if (ImGuiFileDialog::Instance()->Display("SaveFileDlg", ImGuiWindowFlags_None)) {
                if (ImGuiFileDialog::Instance()->IsOk()) {
                    std::string filePath = ImGuiFileDialog::Instance()->GetFilePathName();
                    auto& t = tabs[activeTab];
                    t.path = filePath;
                    t.content = mainEditor.GetText();

                    std::ofstream outFile(filePath);
                    outFile << t.content;
                    outFile.close();

                    t.isDirty = false;
                }
                ImGuiFileDialog::Instance()->Close();
            }
        }
        
        std::string LuaTextEditor::ClearText(const std::string& src)
        {
            std::string s = src;
            while (!s.empty() && (s.back() == '\n' || s.back() == '\r')) {
                s.pop_back();
            }
            return s;
        }
        
        void LuaTextEditor::SwitchTab(int newTab) {
            if (activeTab != -1 && activeTab < (int)tabs.size()) {
                // Salva o conteúdo atual na aba antes de sair
                tabs[activeTab].content = ClearText(mainEditor.GetText());
            }

            activeTab = newTab;

            if (activeTab != -1 && activeTab < (int)tabs.size()) {
                // Carrega o conteúdo da nova aba
                mainEditor.SetText(ClearText(tabs[activeTab].content));
            }
        }
        
        void LuaTextEditor::Render() {
            if (!isOpen) return;

            ImGui::Begin("Scripts Editor", &isOpen, ImGuiWindowFlags_MenuBar);

            // Menu
            if (ImGui::BeginMenuBar()) {
                if (ImGui::BeginMenu("Opções")) {
                    if (ImGui::MenuItem("Abrir Script")) {
                         // se não tiver caminho definido, abrir SaveDialog
                        IGFD::FileDialogConfig cfg;
                        cfg.path = ".";
                        cfg.flags |= ImGuiFileDialogFlags_::ImGuiFileDialogFlags_Modal;
                
                        ImGuiFileDialog::Instance()->OpenDialog("OpenFileDlg", "Abrir Arquivo Lua", ".lua", cfg);
                    }
                    if (ImGui::MenuItem("Salvar Script")) {
                        SaveFile();
                    }
                    if (ImGui::MenuItem("Novo Script")) {
                        AddTab("NovoScript.lua", "-- Novo script\n");
                        mainEditor.SetReadOnly(false);
                    }
                    
                    ImGui::Separator();
                    if (ImGui::MenuItem("Executar") && activeTab >= 0) {
                        tabs[activeTab].content = mainEditor.GetText();
                        interpreter->execute(tabs[activeTab].content);
                    }
                    
                    ImGui::Separator();
                    if (ImGui::MenuItem("Adicionar a Pilha de Execução") && activeTab >= 0) {
                        tabs[activeTab].content = mainEditor.GetText();
                        interpreter->load(tabs[activeTab].path);
                    }
                    
                    ImGui::EndMenu();
                }
                ImGui::EndMenuBar();
            }

            // Tabs
            if (ImGui::BeginTabBar("LuaEditorTabs")) {
                for (int i = 0; i < (int)tabs.size(); i++) {
                    std::string visible = tabs[i].name;
                    if (tabs[i].isDirty) visible += " *";   // mostra * se não salvo
                    std::string tabLabel = visible + "##tab_" + std::to_string(i);

                    if (ImGui::BeginTabItem(tabLabel.c_str(), &tabs[i].open)) {
                        if (activeTab != i) {
                            SwitchTab(i);
                        }
                        ImGui::EndTabItem();
                    }
                    if (!tabs[i].open) {
                        CloseTab(i);
                        i--;
                    }
                }
                ImGui::EndTabBar();
            }

            // Editor
            mainEditor.Render("##MainLuaEditor");

            HandleOpenDialog();
            HandleSaveDialog();
            
            ImGui::End();
        }
    }
}
