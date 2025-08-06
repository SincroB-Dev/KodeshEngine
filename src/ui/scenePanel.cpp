#include "scenePanel.h"

#include "../core/core.h"

#include "../../libs/imgui/imgui.h"
#include "../../libs/ImGuiFileDialog/ImGuiFileDialog.h"

ScenePanel::ScenePanel()
{
    InitShapeFactory();
}

ScenePanel::~ScenePanel()
{}

void ScenePanel::InitShapeFactory()
{
    UI_IOShapes["Shape2DQuad"] = [](Transform2D &t, Color &c) { return std::make_unique<Shape2DQuad>(t, c); };
    UI_IOShapes["Shape2DCircle"] = [](Transform2D &t, Color &c) { return std::make_unique<Shape2DCircle>(t, c); };
    UI_IOShapes["Shape2Star"] = [](Transform2D &t, Color &c) { return std::make_unique<Shape2DStar>(t, c); };

    if (UI_ShapeNames.empty())
    {
        for (const auto &it : UI_IOShapes)
        {
            UI_ShapeNames.push_back(it.first);
        }
    }
}

void ScenePanel::ShapeSelector(SceneManager& scene)
{
    for (size_t i=0; i<UI_ShapeNames.size(); ++i)
    {
        if (ImGui::Selectable(UI_ShapeNames[i].c_str(), UI_ShapeSelectedIndex == i))
        {
            UI_ShapeSelectedIndex = i;

            // Instancia um novo shape no objeto.
            const std::string &selectedName = UI_ShapeNames[i];

            if (UI_IOShapes.count(selectedName))
            {
                std::cout << "Switch to "  << selectedName <<std::endl;
                GameObject *gm = scene.GetMainObject();

                std::unique_ptr<Shape2D> newShape = UI_IOShapes[selectedName](gm->transform, gm->color);

                Shape2D *ptr = newShape.release();
                gm->AttachShape(ptr);
            }
        }
    }
}


void ScenePanel::drawPanel(void *properties)
{
    ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);

    if (ImGui::Button("Carregar Asset"))
        ImGuiFileDialog::Instance()->OpenDialog("FindAsset", "Selecione um Asset", ".*");

    if (ImGuiFileDialog::Instance()->Display("FindAsset"))
    {
        if (ImGuiFileDialog::Instance()->IsOk())
        {
            std::string path = ImGuiFileDialog::Instance()->GetFilePathName();
            // future: usar o path
        }
        ImGuiFileDialog::Instance()->Close();
    }

    // ShapeSelector(scene);
}