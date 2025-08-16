#pragma once

#include <vector>
#include <string>

#include "../../../libs/ImGuiColorTextEdit/TextEditor.h"
#include "../../core/interpreter/luaInterpreter.h"

namespace core {
    namespace ui {
        
        struct ScriptTab {
            std::string name;      // nome da aba (ex: script.lua)
            std::string path;      // caminho real do arquivo
            std::string content;   // conteúdo do script
            bool open = true;      // aba aberta
            bool justOpenedTab = false;
            bool isDirty = false;  // se foi alterado
        };


        class LuaTextEditor {
            std::vector<ScriptTab> tabs;
            TextEditor mainEditor; // único editor
            int activeTab = -1;
            LuaInterpreter interpreter;

        public:
            bool isOpen;
            
            LuaTextEditor();
            ~LuaTextEditor();

            void AddTab(std::string name, const std::string& content = "", const std::string& path = "");
            void CloseTab(int index);

            TextEditor* GetActiveEditor();
            const TextEditor::Palette& GetDefaultTheme();
            
            void SaveFile();
            
            void HandleOpenDialog();
            void HandleSaveDialog();

            void Render();
            void SwitchTab(int newTab);
            std::string ClearText(const std::string& src);
        };

    }
}
