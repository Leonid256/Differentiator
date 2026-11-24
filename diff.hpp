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

//____________________________________________________________________________
const size_t BUFFER_SIZE = 256;
const char* const DATA_FILE  = "data.txt";
const char* const GRAPH_FILE = "tree.dot";
const char* const TEX_FILE   = "tex.txt";

const char* const OPERATIONS[] = {"+", "-", "*", "/", "^", "ln"};
const int len_operations_arr = 4;

const int VAR_CAPACITY = 3;
const char* const VARIABLES[VAR_CAPACITY] = {"x", "y", "z"};

//____________________________________________________________________________
enum operations
{
    ADD,
    SUB,
    MUL,
    DIV,
    POW,
    LN,
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

//____________________________________________________________________________
void delete_subtree(node_t* node);
size_t len_of_file(const char* name);
void preorder_read(const char* const pre_file, tree_t* tree);
node_t* pre_new_node(char* buffer_file, node_t* parent, size_t* size, int* i);

void graph_dump_node(FILE* file, node_t* node, int depth, node_t* current);
void dump_tree(FILE* file, tree_t* tree, node_t* current);
void dump_run(tree_t* tree, node_t** current);

node_t* node_diff(const node_t* node);
#define _DL node_diff(node -> left)
#define _DR node_diff(node -> right)

node_t* new_node(type_node type, tree_elem_t data, node_t* left, node_t* right, node_t* parent);
#define _NUM(data) new_node(NUM, data, NULL, NULL, NULL)
#define _VAR(data) new_node(VAR, data, NULL, NULL, NULL)
#define  _OP(data) new_node(OP,  data, NULL, NULL, NULL)

#define _ADD(node_l, node_r) new_node(OP, ADD, node_l, node_r, NULL)
#define _SUB(node_l, node_r) new_node(OP, SUB, node_l, node_r, NULL)
#define _MUL(node_l, node_r) new_node(OP, MUL, node_l, node_r, NULL)
#define _DIV(node_l, node_r) new_node(OP, DIV, node_l, node_r, NULL)
#define _POW(node_l, node_r) new_node(OP, POW, node_l, node_r, NULL)
#define  _LN(node_l)         new_node(OP, LN,  node_l, NULL,   NULL)

node_t* copy_node(const node_t* node);
#define _CL copy_node(node -> left)
#define _CR copy_node(node -> right)
#define _CN copy_node(node)

void dump_tex(const node_t* node, const char* const tex_file, int line);
void latex_node(FILE* file, const node_t* node);
int priority_op(const node_t* node);
void latex_child(FILE* file, const node_t* child, int parent_prio);


//____________________________________________________________________________
#define DUMP_TEX(node, tex_file) dump_tex(node, tex_file, __LINE__)
#endif