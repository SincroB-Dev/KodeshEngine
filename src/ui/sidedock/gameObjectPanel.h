#ifndef GAMEOBJECT_PANEL_H
#define GAMEOBJECT_PANEL_H

#include "../panel.h"

namespace core
{
    namespace ui
    {
        class GameObjectPanel final : public Panel
        {
        public:
            GameObjectPanel(SceneManager &sm);
            virtual ~GameObjectPanel() override;
        
            void drawPanel(void *properties) override;
        };
    };
};

#endif
