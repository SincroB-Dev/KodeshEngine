#pragma once

#include "../../core/core.h"
#include "../panel.h"

namespace core
{
    namespace ui
    {
        class EnvironmentPanel: public Panel
        {
        public:
            EnvironmentPanel(SceneManager &sm);
            virtual ~EnvironmentPanel() override;
            
            void CameraInfo();
            
            virtual void drawMenu() override;
            virtual void drawPanel(void* properties) override;
        };
    }
}
