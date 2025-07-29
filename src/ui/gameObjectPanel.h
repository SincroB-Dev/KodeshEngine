#ifndef GAMEOBJECT_PANEL_H
#define GAMEOBJECT_PANEL_H

#include "panel.h"

class GameObjectPanel final : public Panel
{
public:
    GameObjectPanel();
    virtual ~GameObjectPanel() override;

    void drawPanel(void *properties) override;
};

#endif