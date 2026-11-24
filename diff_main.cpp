#include "diff.hpp"

int main()
{
    tree_t tree;
    tree.root = NULL;
    tree.size = 0;

    preorder_read(DATA_FILE, &tree);

    // DUMP_TEX(tree.root, TEX_FILE);

    tree.root = node_diff(tree.root);
    conv_tree(&tree);

    dump_run(&tree, &tree.root);
    DUMP_TEX(tree.root, TEX_FILE);

    delete_subtree(tree.root);
}

