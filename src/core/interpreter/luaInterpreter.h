#pragma once
#include <sol/sol.hpp>

class LuaInterpreter {
    void integration();
    
public:
    sol::state lua;

    LuaInterpreter();
    ~LuaInterpreter();

    void execute(const std::string& code);
    void bind(const char* nvar, void* value);
};
