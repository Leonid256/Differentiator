#include "diff.hpp"

int main()
{
    tree_t tree;
    tree.root = NULL;
    tree.size = 0;

    FILE* tex_file = tex_dump_begin(TEX_FILE);
    if (!tex_file) printf(MY_RED "Error in TEX-file open\n" CRESET);

    preorder_read(DATA_FILE, &tree);

    DUMP_TEX(tree.root, tex_file);

    tree.root = node_diff(tree.root);
    conv_tree(&tree);

    dump_run(&tree, &tree.root);
    DUMP_TEX(tree.root, tex_file);

    tex_dump_end(tex_file);
    delete_subtree(tree.root);
}

