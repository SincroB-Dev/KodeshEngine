#include "luaInterpreter.h"

#include <string>
#include <iostream>
#include <sstream>

#include "../../ui/forms/logger.h"

#include "luaIntegration.h"

LuaInterpreter::LuaInterpreter()
{
    // Abre bibliotecas base
    lua.open_libraries(
        sol::lib::base,
        sol::lib::math,
        sol::lib::string,
        sol::lib::table,
        sol::lib::package
    );

    integration();
}

LuaInterpreter::~LuaInterpreter()
{
}

void LuaInterpreter::bind(const char* nvar, void* value)
{
    lua[nvar] = value;
}

void LuaInterpreter::integration()
{
    core::lua::in_enumerators_(lua);
    core::lua::in_classes_(lua);
    core::lua::in_functions_(lua);
}

void LuaInterpreter::load(const std::string& filename)
{
    try
    {
        lua.script_file(filename);
        
        // Aguarda que o script tenha uma função global OnUpdate
        sol::function updateFunc = lua["OnUpdate"];
        
        std::cout << updateFunc.valid() << ", " << updates.size() << std::endl;
        
        if (updateFunc.valid())
        {
            updates[filename] = updateFunc;
            core::ui::LogWindow::Log(("[Lua] Script @'" + filename + "' carregado com sucesso!").c_str(), core::ui::LogType::INFO);
        }
        else
        {
            core::ui::LogWindow::Log(("[Lua] Script @'" + filename + "' não possuí metodo OnUpdate(dt)").c_str(), core::ui::LogType::ERROR);
        }
    }
    catch (const std::exception& e)
    {
        core::ui::LogWindow::Log(("[Lua] Erro ao carregar @'" + filename + "' " + e.what()).c_str(), core::ui::LogType::ERROR);
    }
}

/**
 * Metodo padrão introduzido no loop do game.
 */
void LuaInterpreter::update(float dt)
{
    for (auto map=updates.begin(); map!=updates.end(); map++)
    {
        if (map->second.valid())
        {
            try
            {
                map->second(dt);
                
            } catch (const std::exception& e)
            {
                core::ui::LogWindow::Log((std::string("[Lua] Erro no OnUpdate ") + e.what()).c_str(), core::ui::LogType::ERROR);
            }
        }
    }
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
