#include <sol/sol.hpp>

class LuaInterpreter {
public:
    sol::state lua;

    LuaInterpreter();
    ~LuaInterpreter();

    void execute(const std::string& code);
};
