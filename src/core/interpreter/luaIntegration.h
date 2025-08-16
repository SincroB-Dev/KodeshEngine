#pragma once

#include <sol/sol.hpp>

#include "../../ui/forms/logger.h"

namespace core
{
    namespace lua
    {
        /**
         * Integração de métodos
         */
        void in_functions_(sol::state& lua);
        
        /**
         * Integração de enumeradores
         */
        void in_enumerators_(sol::state& lua);
        
        /**
         * Integração de classes
         */
        void in_classes_(sol::state& lua);
    }
}
