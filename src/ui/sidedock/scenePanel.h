#ifndef SCENEPANEL_H
#define SCENEPANEL_H

#include <string>
#include <memory>
#include <map>
#include <vector>
#include <utility>
#include <functional>

#include "../../models/shapes.h"
#include "../../maths/maths.h"
#include "../../core/core.h"

#include "../panel.h"
#include "../treenode.h"

namespace core
{
    namespace ui
    {
        using ShapeFactory = std::function<std::unique_ptr<Shape2D>(Transform2D &, Color &)>;
        
        class ScenePanel final : public Panel
        {
            // IO -> Instance Objects
            std::map<std::string, ShapeFactory> UI_IOShapes;
        
            size_t UI_ShapeSelectedIndex = -1;
            std::vector<std::string> UI_ShapeNames;
        
        public:
            ScenePanel(SceneManager &sm);
            virtual ~ScenePanel() override;
        
            void InitShapeFactory();
            TreeNode MainHierarchy();
            
            virtual void drawMenu() override;
            virtual void drawPanel(void *properties) override;
        };
    };
};

#endif
