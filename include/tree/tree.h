#ifndef TREE_H
#define TREE_H

//——————————————————————————————————————————————————————————————————————————————————————————

#include "treeTypes.h"
#include "stack.h"
#include <gtest/gtest.h>

#ifdef TREE_DEBUG
    #include "treeGraph.h"
#endif /* TREE_DEBUG */

//——————————————————————————————————————————————————————————————————————————————————————————

#ifdef TREE_DEBUG

#define DEBUG_TREE_CHECK(tree, message, arg)                                                      \
        BEGIN                                                                                     \
        TreeErr_t error = TREE_SUCCESS;                                                           \
        if ((error = TreeCheck(tree, message, __PRETTY_FUNCTION__, __FILE__, __LINE__, arg)))     \
        {                                                                                         \
            return error;                                                                         \
        }                                                                                         \
        END

#define TREE_CALL_DUMP(tree_ptr, message, arg)                                          \
        BEGIN                                                                           \
        TreeDumpInfo_t dump_info = {TREE_SUCCESS, message, __PRETTY_FUNCTION__,         \
                                    __FILE__, __LINE__, arg};                           \
        if (TreeDump(tree_ptr, &dump_info))                                             \
        {                                                                               \
            PRINTERR("DUMPERR");                                                        \
            return TREE_DUMP_ERROR;                                                     \
        }                                                                               \
        END

#else

#define DEBUG_TREE_CHECK(tree, message, arg)      ;
#define TREE_CALL_DUMP(tree_ptr, message, arg)    ;

#endif /* TREE_DEBUG */

//——————————————————————————————————————————————————————————————————————————————————————————

TreeErr_t TreeLoopPrint        (Tree_t*     tree);
TreeErr_t TreeVerify           (Tree_t*     tree);
TreeErr_t TreeSetValuesToArray (Tree_t*     tree, int* array);
TreeErr_t TreeCtor             (Tree_t*     tree);
TreeErr_t TreeNodeCtor         (Tree_t*     tree, TreeElem_t data, TreeNode_t** new_node);
TreeErr_t TreeDtor             (Tree_t*     tree);
TreeErr_t TreeInsert           (Tree_t*     tree, TreeElem_t data);

TreeErr_t TreeNodeVerify       (Tree_t*     tree, TreeNode_t* node, size_t* calls_count);
TreeErr_t TreeSingleNodeDtor   (TreeNode_t*  node    );
TreeErr_t TreeNodeDtor         (TreeNode_t*  node    );
TreeErr_t TreeLeftSubtreeDtor  (TreeNode_t*  node    );
TreeErr_t TreeRightSubtreeDtor (TreeNode_t*  node    );
TreeErr_t TreeSubtreeDtor      (TreeNode_t** node_ptr);

TreeErr_t TreeSetValue         (const TreeNode_t* node, int* array, size_t* i);

TreeErr_t TreeCheck(Tree_t*     tree,
                    const char* func,
                    const char* file,
                    int         line,
                    void*       arg);

//——————————————————————————————————————————————————————————————————————————————————————————

#endif /* TREE_H */
