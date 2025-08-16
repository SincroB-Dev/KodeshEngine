#include "luaIntegration.h"

namespace core
{
    using namespace ui;
    
    namespace lua
    {
        /**
         * Faz chamadas ao log do sistema.
         */
        void kx_log_(const char* msg, LogType type = LogType::INFO)
        {
            LogWindow::Log(msg, type);
        }
        
        /**
         * Integração de métodos
         */
        void in_functions_(sol::state& lua)
        {
            // Utilização de overload para argumento padrão
            lua.set_function("log", 
                sol::overload(
                    [](const char* msg, LogType type){ kx_log_(msg, type); },
                    [](const char* msg){ kx_log_(msg); }
                )
            );
        }
        
        /**
         * Integração de enumeradores
         */
        void in_enumerators_(sol::state& lua)
        {
            // Sistematica de logs
            lua.new_enum("LogType",
                "Info", LogType::INFO,
                "Warn", LogType::WARNING,
                "Error", LogType::ERROR
            );
        }
        
        /**
         * Integração de classes
         */
        void in_classes_(sol::state& lua)
        {
            
        }
    }
}
