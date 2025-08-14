#pragma once

#include <vector>

#include "gizmo2d.h"
#include "grid.h"

#include "../../maths/maths.h"
#include "cameraHandler.h"

class SceneManager;

using namespace maths;

namespace core
{
    namespace editor
    {
        struct Editor
        {
            SceneManager& sm;
            Gizmo2D* gizmos;
            GridView* grid;
            CameraHandler* camHandler;

            Editor(SceneManager* sm);
            ~Editor();

            bool PointInPolygon(vec2 point, const std::vector<vec2> vertices) const;
            void HandleCameraEvents();
            void HandleMouseClick();
            
            void HandleEvents(int mouseX, int mouseY);
            void Update();
        };
    }
}
