#ifndef PANEL_H
#define PANEL_H

class Panel
{
public:
    Panel();
    virtual ~Panel();

    virtual void drawPanel(void *properties);
};

#endif