#include "diff.hpp"

//----------------------------------------------------------------------------
void dump_tex(const node_t* node, const char* const tex_file, int line)
{
    assert(node);

    FILE* file = fopen(tex_file, "w");
    if (!file) return;

    fprintf(file, "\\documentclass{article}\n");
    fprintf(file, "\\title{Differentiator, dump in %d line}\n", line);
    fprintf(file, "\\begin{document}\n");
    fprintf(file, "\\maketitle\n");

    fprintf(file, "\\[");
    latex_node(file, node);
    fprintf(file, "\\]\n");

    fprintf(file, "\\end{document}\n");
}

//----------------------------------------------------------------------------
void latex_node(FILE* file, const node_t* node)
{
    assert(node);

    switch (node -> type)
    {
        case NUM:
            fprintf(file, "{%lg}", node -> num);
            return;
        case VAR:
            fprintf(file, "{%s}", VARIABLES[node -> var]);
            return;
        case OP:
            switch (node -> op)
            {
                case ADD:
                    latex_child(file, node -> left, priority_op(node));
                    fprintf(file, "+");
                    latex_child(file, node -> right, priority_op(node));
                    return;
                case SUB:
                    latex_child(file, node -> left, priority_op(node));
                    fprintf(file, "-");
                    latex_child(file, node -> right, priority_op(node) + 1);
                    return;
                case MUL:
                    latex_child(file, node -> left, priority_op(node));
                    fprintf(file, "\\cdot");
                    latex_child(file, node -> right, priority_op(node));
                    return;
                case DIV:
                    fprintf(file, "\\frac{");
                    latex_node(file, node -> left);
                    fprintf(file, "}{");
                    latex_node(file, node -> right);
                    fprintf(file, "}");
                    return;
                case POW:
                    latex_child(file, node -> left, priority_op(node) + 1);
                    fprintf(file, "^");
                    latex_node(file, node -> right);
                    return;
                case LN:
                    fprintf(file, "\\ln\\left(");
                    latex_node(file, node -> left);
                    fprintf(file, "\\right)");
                    return;
                case NOT_OP:
                default:
                    printf(MY_RED "Error in latex_node: wrong node op\n" CRESET);
                    break;
            }
        default:
            printf(MY_RED "Error in latex_node: wrong node type\n" CRESET);
            break;
    }
}

//----------------------------------------------------------------------------
int priority_op(const node_t* node)
{
    if (node -> type != OP) return 100;

    switch (node -> op)
    {
        case ADD:
        case SUB:
            return 1;
        case MUL:
            return 2;
        case DIV:
            return 3;
        case POW:
            return 4;
        case LN:
        case NOT_OP:
        default:
            return 5;
    }
}

//----------------------------------------------------------------------------
void latex_child(FILE* file, const node_t* child, int parent_prio)
{
    bool brackets = priority_op(child) < parent_prio;
    if (brackets) fprintf(file, "\\left(");
    latex_node(file, child);
    if (brackets) fprintf(file, "\\right)");
}