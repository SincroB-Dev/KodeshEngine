#ifndef PANEL_H
#define PANEL_H

#include "../core/sceneManager.h"

namespace core
{
    namespace ui
    {
        class Panel
        {
        public:
            SceneManager &sceneManager;
            
            Panel(SceneManager &sm);
            virtual ~Panel();

            virtual void drawMenu();
            virtual void drawPanel(void *properties);
            
            virtual SceneManager& GetSM() final;
        };
    }
}
#endif
