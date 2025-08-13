#include "scenePanel.h"

#include "../../core/core.h"
#include "../treenode.h"

#include "../../../libs/imgui/imgui.h"
#include "../../../libs/ImGuiFileDialog/ImGuiFileDialog.h"

namespace core
{
    namespace ui
    {
        ScenePanel::ScenePanel(SceneManager &sm): Panel(sm)
        {
            InitShapeFactory();
        }
        
        ScenePanel::~ScenePanel()
        {}
        
        // Fabrica de shapes
        void ScenePanel::InitShapeFactory()
        {
            UI_IOShapes["Quadrado"] = [](Transform2D& t, Color& c) { return std::make_unique<Shape2DQuad>(t, c); };
            UI_IOShapes["Circulo"] = [](Transform2D& t, Color& c) { return std::make_unique<Shape2DCircle>(t, c); };
            UI_IOShapes["Estrela"] = [](Transform2D& t, Color& c) { return std::make_unique<Shape2DStar>(t, c); };
        
            if (UI_ShapeNames.empty())
            {
                for (const auto &it : UI_IOShapes)
                {
                    UI_ShapeNames.push_back(it.first);
                }
            }
        }
        
        TreeNode ScenePanel::MainHierarchy()
        {
            TreeNode root(sceneManager.GetActiveScene()->name);
            
            auto objLst = sceneManager.GetActiveScene()->GetObjectList();
            
            for (auto it = objLst.begin(); it != objLst.end(); it++)
            {
                root.children.emplace_back((*it)->name);
            }

            return root;
        }
        
        void ScenePanel::drawMenu()
        {
            static bool abrirNomePopup = false;
            static char novoObjetoNome[64] = "Entity";
            
            static std::unique_ptr<GameObject> novoObjetoPtr = nullptr;
            // Detecta clique direito em qualquer lugar, que não seja em uma janela
            // vai abrir o menu de edições, o qual é incrementado por paineis.
            if (ImGui::IsMouseClicked(ImGuiMouseButton_Right) && !ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow)) {
                ImGui::OpenPopup("rcl.toolbox.geral");
            }
            
            // Popup global
            if (ImGui::BeginPopup("rcl.toolbox.geral")) {
                if (ImGui::BeginMenu("Adicionar"))
                {
                    // Shapes disponíveis na aplicação
                    for (size_t i=0; i<UI_ShapeNames.size(); ++i)
                    {
                        if (ImGui::MenuItem(UI_ShapeNames[i].c_str()))
                        {
                            novoObjetoPtr = std::make_unique<GameObject>("GameObject");
                            std::unique_ptr<Shape2D> novoObjetoShape = UI_IOShapes[UI_ShapeNames[i].c_str()](novoObjetoPtr->transform, novoObjetoPtr->color);
                            novoObjetoPtr->AttachShape(novoObjetoShape.release());
                            abrirNomePopup = true;
                        }
                    }
                    
                    ImGui::EndMenu();
                }
                
                ImGui::EndPopup();
            }
            
            // -------------------- POPUP DE NOME --------------------
            if (abrirNomePopup) {
                ImGui::OpenPopup("NovoObjetoPopup");
                abrirNomePopup = false; // evita abrir todo frame
            }

            if (ImGui::BeginPopupModal("NovoObjetoPopup", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
                ImGui::Text("Nome:");
                ImGui::InputText("##NomeObjeto", novoObjetoNome, IM_ARRAYSIZE(novoObjetoNome));

                if (ImGui::Button("OK", ImVec2(120, 0))) {
                    // Aqui você pode criar o objeto com o nome
                    novoObjetoPtr->setName(novoObjetoNome);
                    sceneManager.GetActiveScene()->AddObject(novoObjetoPtr.release());
                    
                    strcpy(novoObjetoNome, "Entity");
                    
                    ImGui::CloseCurrentPopup();
                }
                ImGui::SameLine();
                if (ImGui::Button("Cancelar", ImVec2(120, 0))) {
                    
                    strcpy(novoObjetoNome, "Entity");
                    
                    novoObjetoPtr.reset();
                    
                    ImGui::CloseCurrentPopup();
                }

                ImGui::EndPopup();
            }
        }
        
        // Desenho da ui do painel
        void ScenePanel::drawPanel(void *properties)
        {
            ImGui::Text("Informações");
            ImGui::Text("Framerate: %.1f", ImGui::GetIO().Framerate);

            TreeNode hierarchy = MainHierarchy();
            TreeNode::ShowTreeNodeRecursive(hierarchy);
        }
    };
};
