#ifndef SCENEPANEL_H
#define SCENEPANEL_H

#include "panel.h"

class ScenePanel final : public Panel
{
public:
    ScenePanel();
    virtual ~ScenePanel() override;

    void drawPanel(void *properties) override;
};

#endif