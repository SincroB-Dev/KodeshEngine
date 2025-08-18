#pragma once
#include <sol/sol.hpp>
#include <vector>
#include <map>

class LuaInterpreter {
    void integration();
    
    std::map<std::string, sol::function> updates;
    
public:
    sol::state lua;

    LuaInterpreter();
    ~LuaInterpreter();

    void execute(const std::string& code);
    void update(float dt);
    void load(const std::string& filename);
    void bind(const char* nvar, void* value);
};
