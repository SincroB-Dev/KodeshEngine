#pragma once

#include "scenePanel.h"
#include "gameObjectPanel.h"

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

        public:
            inline DockedToolbox(float wmin = 200.0f, float wmax = 500.0f, float iconbsize = 30.0f, float tabcolwidth = 4.0f)
                : panelWidthMin(wmin), panelWidthMax(wmax), iconButtonSize(iconbsize), tabColumnWidth(iconbsize + tabcolwidth)
            {}

            void DrawIconButton(const char *icon, TBEditorTab tab);
            void Draw(SceneManager& scene);
        };
    };
};