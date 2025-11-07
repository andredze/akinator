#ifndef TREE_TYPES_H
#define TREE_TYPES_H

//——————————————————————————————————————————————————————————————————————————————————————————

#ifdef TREE_DEBUG
    #define DEBUG
#endif

//——————————————————————————————————————————————————————————————————————————————————————————

#include "common.h"

//——————————————————————————————————————————————————————————————————————————————————————————

typedef int TreeElem_t;

#define TREE_SPEC "%d"

const TreeElem_t TREE_POISON = 0xAB0BA;

//——————————————————————————————————————————————————————————————————————————————————————————

typedef struct TreeNode
{
    TreeElem_t       data;
    struct TreeNode* left;
    struct TreeNode* right;
} TreeNode_t;

//——————————————————————————————————————————————————————————————————————————————————————————

typedef struct Tree
{
    TreeNode_t* dummy; // null (fake) element
    size_t      size ;
} Tree_t;

//——————————————————————————————————————————————————————————————————————————————————————————

typedef enum TreeErr
{
    TREE_SUCCESS,
    TREE_NULL,
    TREE_ROOT_NULL,
    TREE_NULL_NODE,
    TREE_NULL_DUMMY,
    TREE_DUMP_ERROR,
    TREE_CALLOC_ERROR,
    TREE_SIZE_EXCEEDS_MAX,
    TREE_STACK_ERROR,
    TREE_SYSTEM_FUNC_ERR
} TreeErr_t;

//——————————————————————————————————————————————————————————————————————————————————————————

#ifdef TREE_DEBUG

//——————————————————————————————————————————————————————————————————————————————————————————

const char* const TREE_STR_ERRORS[] =
{
    [TREE_SUCCESS]            = "Tree is ok",
    [TREE_NULL]               = "Tree is a nullptr",
    [TREE_ROOT_NULL]          = "Root of the tree is a nullptr",
    [TREE_NULL_NODE]          = "Given node is a nullptr",
    [TREE_NULL_DUMMY]         = "Tree dummy is a nullptr",
    [TREE_DUMP_ERROR]         = "Error with dumping tree",
    [TREE_CALLOC_ERROR]       = "Memory allocation for tree failed",
    [TREE_SIZE_EXCEEDS_MAX]   = "Tree size exceeded maximum possible value",
    [TREE_STACK_ERROR]        = "Stack operation failed",
    [TREE_SYSTEM_FUNC_ERR]    = "System function failed"
};

//——————————————————————————————————————————————————————————————————————————————————————————

typedef struct TreeDumpInfo
{
    TreeErr_t   error;
    const char* reason;
    const char* func;
    const char* file;
    int         line;
    void*       command_arg;
} TreeDumpInfo_t;

//——————————————————————————————————————————————————————————————————————————————————————————

#endif /* TREE_DEBUG */

//——————————————————————————————————————————————————————————————————————————————————————————

const size_t TREE_MAX_SIZE = 1024 * 1024 * 1024;

//——————————————————————————————————————————————————————————————————————————————————————————

#endif /* TREE_TYPES_H */
