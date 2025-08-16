#include "editor.h"
#include "../sceneManager.h"
#include "../gameObject.h"

namespace core
{
    namespace editor
    {
        Editor::Editor(SceneManager& sm): sm(sm)
        {
            gizmos = new Gizmo2D(&sm);
            grid = new GridView();
            camHandler = new CameraHandler(this, 0.015f);
        }

        Editor::~Editor()
        {
            delete gizmos;
            delete grid;
            delete camHandler;
        }

        bool Editor::PointInPolygon(vec2 point, const std::vector<vec2> vertices) const
        {
            bool inside = false;
            for (size_t i=0, j=vertices.size() - 1; i<vertices.size(); j = i++)
            {
                bool intersect = ((vertices[i].y > point.y) != (vertices[j].y > point.y)) &&
                            (point.x < (vertices[j].x - vertices[i].x) * (point.y - vertices[i].y) /
                            (vertices[j].y - vertices[i].y) - vertices[i].x);

                if (intersect)
                {
                    inside = !inside;
                }
            }
            return inside;
        }

        void Editor::HandleEvents(int mouseX, int mouseY)
        {
            gizmos->Update();
            
            camHandler->Movement(mouseX, mouseY);
            camHandler->Zoom();
        }
        
        void Editor::Update()
        {
            
        }

        void Editor::HandleMouseClick()
        {
            vec2 worldMouse = sm.GetActiveCamera()->GetWorldMouse();

            for (auto& obj : sm.GetActiveScene()->GetObjectList())
            {
                if (GameObject* gobj = dynamic_cast<GameObject*>(obj))
                {
                    vec2& pos = gobj->transform.localPosition;
                    std::vector<vec2> final;

                    for (auto& v : gobj->shape->vertices)
                    {
                        final.push_back(
                            vec2(v.x - pos.x, v.y + pos.y)
                        );
                    }

                    if (PointInPolygon(worldMouse, final))
                    {
                        sm.GetActiveScene()->activeObject = obj;
                    }
                }
            }
        }
    }
}
