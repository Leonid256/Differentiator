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
                        if (node -> right -> type == VAR && node -> right -> var == 0)
                            return _MUL(_MUL(_CN, _LN(_CL)), _DR);
                        else
                            return _NUM(0);
                    }
                    break;
                case LN:
                    return _MUL(_DIV(_NUM(1), _CN), _DL);
                case SIN:
                    return _MUL(_COS(_CL), _DL);
                case COS:
                    return _MUL(_MUL(_SIN(_CL), _DL), _NUM(-1));
                case TG:
                    return _MUL(_DIV(_NUM(1), _POW(_COS(_CL), _NUM(2))), _DL);
                case CTG:
                    return _MUL(_DIV(_NUM(-1), _POW(_SIN(_CL), _NUM(2))), _DL);
                case NOT_OP:
                default:
                    printf(MY_RED "Error in node_diff: wrong node op\n" CRESET);
                    break;
            }
            break;
        default:
            printf(MY_RED "Error in node_diff: wrong node type\n" CRESET);
            break;
    }

    return NULL;
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
            node -> var = (int)data;
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

//----------------------------------------------------------------------------
void make_num(node_t* node, double value)
{
    delete_subtree(node -> left);
    delete_subtree(node -> right);

    node -> left  = NULL;
    node -> right = NULL;
    node -> type  = NUM;
    node -> num   = value;
}

//----------------------------------------------------------------------------
void replace_child(node_t* node, node_t* child, node_t* other)
{
    assert(node);
    assert(child && other);

    delete_subtree(other);

    node -> type = child  -> type;
    node -> num = child   -> num;
    node -> left = child  -> left;
    node -> right = child -> right;

    if (node -> left)  node -> left  -> parent = node;
    if (node -> right) node -> right -> parent = node;

    free(child);
}

//----------------------------------------------------------------------------
bool conv_const(node_t* node)
{
    if (!node) return false;

    bool changed = false;
    changed |= conv_const(node -> left);
    changed |= conv_const(node -> right);

    if (node -> type != OP) return changed;

    node_t* left  = node -> left;
    node_t* right = node -> right;

    if (count_func(node, &changed))
        return changed;

    if (count_op(node, &changed))
        return changed;

    switch (node -> op)
    {
        case ADD:
            if (is_val(left, 0))
            {
                replace_child(node, right, left);
                return true;
            }
            if (is_val(right, 0))
            {
                replace_child(node, left, right);
                return true;
            }
            break;
        case SUB:
            if (is_val(right, 0))
            {
                replace_child(node, left, right);
                return true;
            }
            break;
        case MUL:
            if (is_val(left, 0) || is_val(right, 0))
            {
                make_num(node, 0);
                return true;
            }
            if (is_val(left, 1))
            {
                replace_child(node, right, left);
                return true;
            }
            if (is_val(right, 1))
            {
                replace_child(node, left, right);
                return true;
            }
            break;
        case DIV:
            if (is_val(left, 0))
            {
                make_num(node, 0);
                return true;
            }
            if (is_val(right, 1))
            {
                replace_child(node, left, right);
                return true;
            }
            break;
        case POW:
            if (is_val(right, 0))
            {
                make_num(node, 1);
                return true;
            }
            if (is_val(right, 1))
            {
                replace_child(node, left, right);
                return true;
            }
            break;
        case SIN:
            if (is_val(left, 0))
            {
                make_num(node, 0);
                return true;
            }
        case COS:
            if (is_val(left, 0))
            {
                make_num(node, 1);
                return true;
            }
        case TG:
            if (is_val(left, 0))
            {
                make_num(node, 0);
                return true;
            }
        case CTG:
        case LN:
        case NOT_OP:
        default:
            break;
    }

    return changed;
}

//----------------------------------------------------------------------------
bool is_val(node_t* node, tree_elem_t value)
{
    return node && node -> type == NUM && fabs(node -> num - value) < EPS;
}

//----------------------------------------------------------------------------
void conv_tree(tree_t* tree)
{
    while(conv_const(tree -> root))
        ;
}

//----------------------------------------------------------------------------
bool count_func(node_t* node, bool* changed)
{
    if (!node)
    {
        *changed = false;
        return true;
    }

    node_t* left  = node -> left;

    if (node -> op == LN)
    {
        if (left -> type == NUM && left -> num > 0)
        {
            make_num(node, log(left -> num));
            *changed = true;
            return true;
        }

        return true;
    }
    if (node -> op == SIN)
    {
        if (left -> type == NUM)
        {
            make_num(node, sin(left -> num));
            *changed = true;
            return true;
        }

        return true;
    }
    if (node -> op == COS)
    {
        if (left -> type == NUM)
        {
            make_num(node, cos(left -> num));
            *changed = true;
            return true;
        }

        return true;
    }
    if (node -> op == TG)
    {
        if (left -> type == NUM)
        {
            make_num(node, tan(left -> num));
            *changed = true;
            return true;
        }

        return true;
    }
    if (node -> op == CTG)
    {
        if (left -> type == NUM)
        {
            make_num(node, cos(left -> num) / sin(left -> num));
            *changed = true;
            return true;
        }

        return true;
    }

    return false;
}

//----------------------------------------------------------------------------
bool count_op(node_t* node, bool* changed)
{
    node_t* left  = node -> left;
    node_t* right = node -> right;

    if (left -> type == NUM && right -> type == NUM)
    {
        switch (node -> op)
        {
            case ADD:
                make_num(node, left -> num + right -> num);
                *changed = true;
                return true;
            case SUB:
                make_num(node, left -> num - right -> num);
                *changed = true;
                return true;
            case MUL:
                make_num(node, left -> num * right -> num);
                *changed = true;
                return true;
            case DIV:
                if (!is_val(right, 0))
                {
                    make_num(node, left -> num / right -> num);
                    *changed = true;
                    return true;
                }
                break;
            case POW:
                make_num(node, pow(left -> num, right -> num));
                *changed = true;
                return true;
            case LN:
            case SIN:
            case COS:
            case TG:
            case CTG:
            case NOT_OP:
            default:
                break;
        }
    }

    return false;
}