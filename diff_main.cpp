#include "diff.hpp"

int main()
{
    tree_t tree;
    tree.root = NULL;
    tree.size = 0;

    preorder_read(DATA_FILE, &tree);

    dump_run(&tree, &tree.root);
}