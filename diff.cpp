#include "diff.hpp"

//----------------------------------------------------------------------------
node_t* node_diff(const node_t* node)
{
    assert(node);

    switch(node -> type)
    {
        case NUM:
            return _NUM(0);
        case VAR:
            return _NUM(1);
        case OP:
            switch(node -> op)
            {
                case ADD:
                    return _ADD(_DL, _DR);
                case SUB:
                    return _SUB(_DL, _DR);
                case MUL:
                    return _ADD(_MUL(_DL, _CR),
                                _MUL(_CL, _DR));
                case DIV:
                    return _DIV(_SUB(_MUL(_DL, _CR), _MUL(_CL, _DR)),
                                _POW(_CR, _NUM(2)));
                case POW:
                    if (node -> left -> type == VAR && node -> left -> var == 0)
                    {
                        if (node -> right -> type == NUM)
                        {
                            return _MUL(_MUL(_NUM(node -> right -> num),
                                        _POW(_CL, _NUM(node -> right -> num - 1))),
                                        _DL);
                        }
                    }
                    else if (node -> left -> type == NUM)
                    {
                        if (node -> right -> type == VAR && node -> left -> var == 0)
                            return _MUL(_MUL(_CN, _LN(_CL)), _DR);
                        else
                            return _NUM(0);
                    }
                case LN:
                    return _MUL(_DIV(_NUM(1), _CN), _DL);
            }
    }
}

//----------------------------------------------------------------------------
node_t* new_node(type_node type, tree_elem_t data, node_t* left, node_t* right, node_t* parent)
{
    node_t* node   = (node_t*)calloc(1, sizeof(node_t));
    assert(node);

    node -> type   = type;
    node -> left   = left;
    node -> right  = right;
    node -> parent = parent;

    switch(type)
    {
        case NUM:
            node -> num = (tree_elem_t)data;
            break;
        case VAR:
            node -> var = data;
            break;
        case OP:
            node -> op = (operations)data;
            break;
        default:
            printf(MY_RED "Error in new_node: unknown type\n" CRESET);
            break;
    }

    if (left)  left  -> parent = node;
    if (right) right -> parent = node;

    return node;
}

//----------------------------------------------------------------------------
node_t* copy_node(const node_t* node)
{
    if(!node) return NULL;

    node_t* copy = (node_t*)calloc(1, sizeof(node_t));
    assert(copy);

    copy -> type = node -> type;
    switch(node -> type)
    {
        case(NUM):
            copy -> num = node -> num;
            break;
        case(VAR):
            copy -> var = node -> var;
            break;
        case(OP):
            copy -> op  = node -> op;
            break;
        default:
            printf(MY_RED "Error in copy_node: wrong type\n" CRESET);
            break;
    }

    copy -> left  = copy_node(node -> left);
    copy -> right = copy_node(node -> right);

    if (copy -> left)  copy -> left  -> parent = copy;
    if (copy -> right) copy -> right -> parent = copy;

    return(copy);
}
