#include "diff.hpp"

//----------------------------------------------------------------------------
void delete_subtree(node_t* node)
{
    if (!node) return;

    delete_subtree(node -> left);
    delete_subtree(node -> right);

    free(node);
}

//----------------------------------------------------------------------------
size_t len_of_file(const char* name)
{
    assert(name);
    
    struct stat file_info;
    stat(name, &file_info);

    if (file_info.st_size <= 0) return 0;
    else return (size_t)file_info.st_size;
}

//----------------------------------------------------------------------------
void preorder_read(const char* const pre_file, tree_t* tree)
{
    assert(pre_file);
    assert(tree);

    FILE* file = fopen(pre_file, "r");
    if (!file)
    {
        printf(MY_RED "Error in preorder_read\n" CRESET);
        return;
    }

    size_t file_size = len_of_file(pre_file);
    char* buffer_file = (char*)calloc(file_size + 2, 1);
    fread(buffer_file, 1, file_size, file);
    fclose(file);

    int i = 0;
    while (isspace((unsigned char)buffer_file[i])) i++;

    if (buffer_file[i++] != '(')
    {
        printf(MY_RED "Error in preorder_read\n" CRESET);
        free(buffer_file);
        return;
    }
    
    size_t size = 0;
    node_t* root = pre_new_node(buffer_file, NULL, &size, &i);
    if (root)
        tree -> root = root;
    else
    {
        printf(MY_RED "Error in preorder_read\n" CRESET);
        free(buffer_file);
        return;
    }
    
    tree -> size = size;

    free(buffer_file);
    printf(MY_GRN "Data downloaded successfully\n" CRESET);
}

//----------------------------------------------------------------------------
node_t* pre_new_node(char* buffer_file, node_t* parent, size_t* size, int* i)
{
    node_t* node = (node_t*) calloc(1, sizeof(node_t));
    if (node == NULL)
    {
        printf(MY_RED "Error in pre_new_node\n" CRESET);
        return NULL;
    }

    char* buffer_data = (char*)calloc(BUFFER_SIZE, 1);

    int buf_i = 0;
    while(buffer_file[*i] != '\0' && buffer_file[*i] != '(' && buffer_file[*i] != ')')
    {
        buffer_data[buf_i++] = buffer_file[*i];
        (*i)++;
    }
    buffer_data[buf_i] = '\0';

    //проверка типа узла
    bool op  = false;
    bool var = false;
    for (int i = 0; i < len_operations_arr || i < VAR_CAPACITY; i++)
    {
        if (i < len_operations_arr && strcmp(buffer_data, OPERATIONS[i]) == 0)
        {
            node -> type = OP;
            node -> op  = (operations)i;

            op = true;
            break;
        }
        else if (i < VAR_CAPACITY && strcmp(buffer_data, VARIABLES[i]) == 0)
        {
            node -> type = VAR;
            node -> var = i;

            var = true;
            break;
        }
    }
    if (!op && !var)
    {
        node -> type = NUM;
        node -> num = strtod(buffer_data, NULL);
    }

    node -> parent = parent;
    (*size)++;

    while(buffer_file[*i] != '\0' && buffer_file[*i] != '(' && buffer_file[*i] != ')') (*i)++;
    if (buffer_file[*i] == ')')
    {
        node -> left = NULL;
        node -> right = NULL;
        (*i)++;
        return node;
    }
    else if (buffer_file[*i] == '(')
    {
        (*i)++;
        node -> left = pre_new_node(buffer_file, node, size, i);
        if (!node -> left)
        {
            delete_subtree(node);
            return NULL;
        }
    }
    else
    {
        printf(MY_RED "Error in pre_new_node\n" CRESET);
        return NULL;
    }

    while(buffer_file[*i] != '\0' && buffer_file[*i] != '(' && buffer_file[*i] != ')') (*i)++;
    if (buffer_file[*i] == ')')
    {
        printf(MY_RED "Error in pre_new_node: node has only one child\n" CRESET);
        delete_subtree(node);
        return NULL;
    }
    else if (buffer_file[*i] == '(')
    {
        (*i)++;
        node -> right = pre_new_node(buffer_file, node, size, i);
        if (!node -> right)
        {
            delete_subtree(node);
            return NULL;
        }
    }
    else
    {
        printf(MY_RED "Error in pre_new_node\n" CRESET);
        return NULL;
    }

    if (buffer_file[*i] != ')')
    {
        printf(MY_RED "Error in pre_new_node\n" CRESET);
        return NULL;
    }
    
    (*i)++;
    return node;
}