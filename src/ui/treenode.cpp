#include "treenode.h"
#include "../../libs/imgui/imgui.h"

namespace core
{
    namespace ui
    {
        void TreeNode::ShowTreeNodeRecursive(TreeNode &node)
        {
            ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
            if (node.children.empty())
                flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
            if (node.selected)
                flags |= ImGuiTreeNodeFlags_Selected;

            bool open = ImGui::TreeNodeEx(node.name.c_str(), flags);

            // Clique com botão esquerdo = seleção
            if (ImGui::IsItemClicked())
            {
                node.selected = true;
            }

            if (open && !node.children.empty())
            {
                for (auto &child : node.children)
                {
                    ShowTreeNodeRecursive(child);
                }
                ImGui::TreePop();
            }
        }

        // Exemplo de construção de árvore
        TreeNode CreateExampleTree()
        {
            TreeNode root("Root");
            root.children.emplace_back("Child A");
            root.children.emplace_back("Child B");

            TreeNode &childB = root.children.back();
            childB.children.emplace_back("Sub B1");
            childB.children.emplace_back("Sub B2");

            TreeNode &subB2 = childB.children.back();
            subB2.children.emplace_back("Sub B2.1");

            return root;
        }

        // No loop do ImGui
        void ShowExampleTreeViewer()
        {
            static TreeNode root = CreateExampleTree();

            //ImGui::Begin("Tree Viewer");
            TreeNode::ShowTreeNodeRecursive(root);
            //ImGui::End();
        }
    }
}