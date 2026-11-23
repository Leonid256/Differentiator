#include "diff.hpp"

//----------------------------------------------------------------------------
void graph_dump_node(FILE* file, node_t* node, int depth, node_t* current)
{
    if (!node) return;

    const char* colour = "lightgrey";
    if (node -> left == NULL || node -> right == NULL) colour = "green";
    if (node -> left == NULL && node -> right == NULL) colour = "lightgreen";
    if (node -> parent == NULL) colour = "lightblue";
    if ((node -> left  && node -> left -> parent  != node) || (node -> right && node -> right -> parent != node))
        colour = "red";
    if (node == current) colour = "yellow";

    switch(node -> type)
    {
        case OP:
            fprintf(file, "\tn_%p [style = filled, fillcolor = %s, label = \"{ %p | TYPE = %d(OP) | DATA(op) = %d | PARENT = %p | { <l> LEFT = %p | <r> RIGHT = %p}}\"];\n",
                (void*)node, colour, (void*)node, node -> type, node -> op, (void*)node -> parent, 
                (void*)node -> left, (void*)node -> right);
            break;
        case VAR:
            fprintf(file, "\tn_%p [style = filled, fillcolor = %s, label = \"{ %p | TYPE = %d(VAR) | DATA(var) = %d | PARENT = %p | { <l> LEFT = %p | <r> RIGHT = %p}}\"];\n",
                (void*)node, colour, (void*)node, node -> type, node -> var, (void*)node -> parent, 
                (void*)node -> left, (void*)node -> right);
            break;
        case NUM:
            fprintf(file, "\tn_%p [style = filled, fillcolor = %s, label = \"{ %p | TYPE = %d(NUM) | DATA(num) = %lg | PARENT = %p | { <l> LEFT = %p | <r> RIGHT = %p}}\"];\n",
                (void*)node, colour, (void*)node, node -> type, node -> num, (void*)node -> parent, 
                (void*)node -> left, (void*)node -> right);
            break;
    }

    if (node -> left)
    {
        fprintf(file, "\tn_%p:l -> n_%p;\n", (void*)node, (void*)node -> left);
        graph_dump_node(file, node -> left, depth + 1, current);
    }
    else
    {
        fprintf(file, "\tnl_%p [style = invis, label = \"\", width = 0];\n", (void*)node);
        fprintf(file, "\tn_%p:l -> nl_%p [style = invis];\n", (void*)node, (void*)node);
    }

    if (node -> right)
    {
        fprintf(file, "\tn_%p:r -> n_%p;\n", (void*)node, (void*)node -> right);
        graph_dump_node(file, node -> right, depth + 1, current);
    }
    else
    {
        fprintf(file, "\tnr_%p [style = invis, label = \"\", width = 0];\n", (void*)node);
        fprintf(file, "\tn_%p:r -> nr_%p [style = invis];\n", (void*)node, (void*)node);
    }
}

//----------------------------------------------------------------------------
void dump_tree(FILE* file, tree_t* tree, node_t* current)
{
    assert(file && tree);

    fprintf(file, "digraph Tree\n{\n");
    fprintf(file, "\tordering = out;\n");
    fprintf(file, "\tnode [shape = record];\n");

    graph_dump_node(file, tree -> root, 0, current);

    fprintf(file, "}\n");
}

//----------------------------------------------------------------------------
void dump_run(tree_t* tree, node_t** current)
{
    assert(tree);
    
    FILE* dump_file = fopen(GRAPH_FILE, "w");
    if (!dump_file)
    {
        printf(MY_RED "Error: can't open %s\n" CRESET, GRAPH_FILE);
        return;
    }

    dump_tree(dump_file, tree, *current);
    fclose(dump_file);

    if (system("dot -Tsvg tree.dot -o tree.svg") != 0)
    {
        printf(MY_RED "Error: dot failed\n" CRESET);
        return;
    }
}