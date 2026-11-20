#ifndef DIFF_HPP
#define DIFF_HPP

//----------------------------------------------------------------------------
typedef double tree_elem_t;

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/stat.h>
#include <ctype.h>
#include <string.h>
#include <math.h>

#include "colours.hpp"

//----------------------------------------------------------------------------
const size_t BUFFER_SIZE = 256;
const char* const DATA_FILE = "data.txt";
const char* const GRAPH_FILE = "tree.dot";

const char* const OPERATIONS[] = {"+", "-", "*", "/"};
const int len_operations_arr = 4;

const int VAR_CAPACITY = 3;
const char* const VARIABLES[VAR_CAPACITY] = {"x", "y", "z"};

//----------------------------------------------------------------------------
enum operations
{
    ADD,
    SUB,
    MUL,
    DIV,
    NOT_OP,
};

enum type_node
{
    NUM,
    VAR,
    OP,
};

struct node_t
{
    type_node type;
    union
    {
        operations op;
        int var;
        tree_elem_t num;
    };
    node_t* left;
    node_t* right;
    node_t* parent;
};

struct tree_t
{
    node_t* root;
    size_t size;
};

//----------------------------------------------------------------------------
void delete_subtree(node_t* node);
size_t len_of_file(const char* name);
void preorder_read(const char* const pre_file, tree_t* tree);
node_t* pre_new_node(char* buffer_file, node_t* parent, size_t* size, int* i);
void graph_dump_node(FILE* file, node_t* node, int depth, node_t* current);
void dump_tree(FILE* file, tree_t* tree, node_t* current);
void dump_run(tree_t* tree, node_t** current);

#endif