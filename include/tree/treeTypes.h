#ifndef TREE_TYPES_H
#define TREE_TYPES_H

//——————————————————————————————————————————————————————————————————————————————————————————

#ifdef TREE_DEBUG
    #define DEBUG
#endif

//——————————————————————————————————————————————————————————————————————————————————————————

#include "common.h"

//——————————————————————————————————————————————————————————————————————————————————————————

typedef char* TreeElem_t;

#define TREE_SPEC "%s"

const TreeElem_t TREE_POISON = NULL;

//——————————————————————————————————————————————————————————————————————————————————————————

typedef struct TreeNode
{
    TreeElem_t       data ;
    struct TreeNode* left ;
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
    TREE_SUCCESS = 0,
    TREE_NULL,
    TREE_NULL_NODE,
    TREE_NULL_DUMMY,
    TREE_SIZE_EXCEEDS_MAX,
    TREE_LOOP,

    TREE_CALLOC_ERROR,
    TREE_DUMP_ERROR,
    TREE_SYSTEM_FUNC_ERR,
    TREE_FILE_ERR,
    TREE_STACK_ERR,

    TREE_INVALID_INPUT
} TreeErr_t;

//——————————————————————————————————————————————————————————————————————————————————————————

#ifdef TREE_DEBUG

//——————————————————————————————————————————————————————————————————————————————————————————

const char* const TREE_STR_ERRORS[] =
{
    [TREE_SUCCESS]            = "Tree is ok",
    [TREE_NULL]               = "Tree is a nullptr",
    [TREE_NULL_NODE]          = "Given node is a nullptr",
    [TREE_NULL_DUMMY]         = "Tree dummy is a nullptr",
    [TREE_SIZE_EXCEEDS_MAX]   = "Tree size exceeded maximum possible value",
    [TREE_LOOP]               = "Tree is looped",
    [TREE_CALLOC_ERROR]       = "Memory allocation for tree failed",
    [TREE_DUMP_ERROR]         = "Error with dumping tree",
    [TREE_SYSTEM_FUNC_ERR]    = "System function failed",
    [TREE_FILE_ERR]           = "Error with opening/writing to file",
    [TREE_STACK_ERR]          = "Error with stack commands",
    [TREE_INVALID_INPUT]      = "User input is invalid"
};

//——————————————————————————————————————————————————————————————————————————————————————————

typedef struct TreeDumpInfo
{
    TreeErr_t   error;
    const char* func;
    const char* file;
    int         line;
} TreeDumpInfo_t;

//——————————————————————————————————————————————————————————————————————————————————————————

#endif /* TREE_DEBUG */

//——————————————————————————————————————————————————————————————————————————————————————————

const size_t TREE_MAX_SIZE = 1024 * 1024 * 1024;

//——————————————————————————————————————————————————————————————————————————————————————————

#endif /* TREE_TYPES_H */
