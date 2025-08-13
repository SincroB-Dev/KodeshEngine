#pragma once

#include <vector>
#include <string>

namespace core
{
    namespace ui
    {
        // Representação básica de um nó
        struct TreeNode
        {
            std::string name;
            std::vector<TreeNode> children;
            bool selected = false;

            TreeNode(const std::string &n) : name(n) {}

            static void ShowTreeNodeRecursive(TreeNode &node);
        };

        TreeNode CreateExampleTree();
        void ShowExampleTreeViewer();
    }
}
