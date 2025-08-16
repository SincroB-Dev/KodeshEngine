#pragma once

#include "scenePanel.h"
#include "gameObjectPanel.h"
#include "environmentInfoPanel.h"

#include "../../core/sceneManager.h"

namespace core
{
    namespace ui
    {
        enum class TBEditorTab
        {
            Ambiente,
            Cenario,
            Entidade,
            Material,
            Componentes
        };

        class DockedToolbox
        {
            TBEditorTab activeTab = TBEditorTab::Ambiente;

            float panelWidth = 300.0f;

            const float panelWidthMin = 200.0f;
            const float panelWidthMax = 500.0f;

            const float iconButtonSize;
            const float tabColumnWidth;

            // Paineis
            ScenePanel scenePanel;
            GameObjectPanel entityPanel;
            EnvironmentPanel environmentPanel;
            
            // Gerenciador da Cena
            SceneManager& sceneManager;

        public:
            inline DockedToolbox(SceneManager &sm, float wmin = 200.0f, float wmax = 500.0f, float iconbsize = 30.0f, float tabcolwidth = 4.0f)
                : panelWidthMin(wmin), panelWidthMax(wmax), 
                    iconButtonSize(iconbsize), tabColumnWidth(iconbsize + tabcolwidth), 
                    scenePanel(sm), entityPanel(sm), environmentPanel(sm), sceneManager(sm)
            {}
            virtual ~DockedToolbox() {};

            void DrawIconButton(const char *icon, TBEditorTab tab);
            void Draw(float& menuHeight);
        };
    };
};
