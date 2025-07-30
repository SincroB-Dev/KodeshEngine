#pragma once

#include "../../maths/maths.h"

namespace scene
{
    namespace editor
    {
        
        class GridView
        {
        public:
            int grid_size = 5;

            Color grid_color = {0.36f, 0.36f, 0.36f, 1.0f};
            Color x_axis_color = {1.0f, 0.0f, 0.0f, 1.0f};
            Color y_axis_color = {0.0f, 1.0f, 0.0f, 1.0f};

            void Draw();
        };

    };
};