#include "luaInterpreter.h"

#include <string>
#include <iostream>
#include <sstream>

#include "../../ui/forms/logger.h"

LuaInterpreter::LuaInterpreter()
{
    // Abre bibliotecas base
    lua.open_libraries(sol::lib::base,
                        sol::lib::math,
                        sol::lib::string,
                        sol::lib::table,
                        sol::lib::package);
}

LuaInterpreter::~LuaInterpreter()
{
}

void LuaInterpreter::execute(const std::string& code)
 {
    sol::load_result script = lua.load(code);
    
    if (!script.valid()) {
        sol::error err = script;
        std::cerr << "Lua syntax error: " << err.what() << "\n";
        core::ui::LogWindow::Log("Lua, erro de sintaxe!", core::ui::LogType::ERROR);
        return;
    }
    sol::protected_function_result result = script();
    if (!result.valid()) {
        sol::error err = result;
        std::cerr << "Lua runtime error: " << err.what() << "\n";
        core::ui::LogWindow::Log("Lua, erro de execução!", core::ui::LogType::ERROR);
    }
}
