#include "panel.h"

namespace core
{
    namespace ui
    {
        Panel::Panel(SceneManager &sm): sceneManager(sm) 
        {
        
        }
        
        Panel::~Panel()
        {
        
        }
        
        void Panel::drawMenu()
        {
            
        }
        
        void Panel::drawPanel(void *properties)
        {
        
        }
        
        SceneManager& Panel::GetSM()
        {
            return this->sceneManager;
        }
    }
}
