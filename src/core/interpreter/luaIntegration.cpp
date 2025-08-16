#include "luaIntegration.h"

namespace core
{
    using namespace ui;
    
    namespace lua
    {
        /**
         * Faz chamadas ao log do sistema.
         */
        void kx_log_(const char* msg, LogType type)
        {
            LogWindow::Log(msg, type);
        }
        
        /**
         * Integração de métodos
         */
        void in_functions_(sol::state& lua)
        {
            lua.set_function("log", &kx_log_);
        }
        
        /**
         * Integração de enumeradores
         */
        void in_enumerators_(sol::state& lua)
        {
            // Sistematica de logs
            lua.create_named_table("LogType",
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
