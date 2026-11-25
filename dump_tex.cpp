#include "diff.hpp"

//----------------------------------------------------------------------------
FILE* tex_dump_begin(const char* tex_file)
{
    assert(tex_file);

    FILE* file = fopen(tex_file, "w");
    if (!file)
    {
        printf(MY_RED "Error in TEX-file open\n" CRESET);
        return NULL;
    }

    fprintf(file, "\\documentclass{article}\n");
    fprintf(file, "\\title{Differentiator Dump}\n");
    fprintf(file, "\\begin{document}\n");
    fprintf(file, "\\maketitle\n\n");

    return file;
}

//----------------------------------------------------------------------------
void tex_dump_end(FILE* file)
{
    if (!file) return;

    fprintf(file, "\\end{document}\n");
    fclose(file);
}

//----------------------------------------------------------------------------
void dump_tex(const node_t* node, FILE* file, int line)
{
    assert(node);

    if (!file) return;

    fprintf(file, "\\subsection{Differentiator, dump in %d line}\n", line);

    fprintf(file, "\\[");
    latex_node(file, node);
    fprintf(file, "\\]\n\n");
}

//----------------------------------------------------------------------------
void latex_node(FILE* file, const node_t* node)
{
    assert(file);
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
                case SIN:
                    fprintf(file, "\\sin\\left(");
                    latex_node(file, node -> left);
                    fprintf(file, "\\right)");
                    return;
                case COS:
                    fprintf(file, "\\cos\\left(");
                    latex_node(file, node -> left);
                    fprintf(file, "\\right)");
                    return;
                case TG:
                    fprintf(file, "\\tan\\left(");
                    latex_node(file, node -> left);
                    fprintf(file, "\\right)");
                    return;
                case CTG:
                    fprintf(file, "\\cot\\left(");
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
    assert(node);

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
        case SIN:
        case COS:
        case TG:
        case CTG:
            return 5;
        case NOT_OP:
        default:
            return 6;
    }
}

//----------------------------------------------------------------------------
void latex_child(FILE* file, const node_t* child, int parent_prio)
{
    assert(file);
    assert(child);

    bool brackets = priority_op(child) < parent_prio;
    if (brackets) fprintf(file, "\\left(");
    latex_node(file, child);
    if (brackets) fprintf(file, "\\right)");
}