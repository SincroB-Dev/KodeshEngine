#ifndef PANEL_H
#define PANEL_H

namespace core
{
    namespace ui
    {
        class Panel
        {
        public:
            Panel();
            virtual ~Panel();

            virtual void drawPanel(void *properties);
        };
    }
}
#endif