#ifndef TREE_GRAPH_H
#define TREE_GRAPH_H

//——————————————————————————————————————————————————————————————————————————————————————————

#include "treeTypes.h"
#include <time.h>
#include <sys/stat.h>

//——————————————————————————————————————————————————————————————————————————————————————————

#ifdef TREE_DEBUG

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

#define TREE_CALL_DUMP(tree_ptr, name, message, arg)    ;

#endif /* TREE_DEBUG */

//——————————————————————————————————————————————————————————————————————————————————————————

TreeErr_t TreeDump            (const Tree_t*     tree, const TreeDumpInfo_t* dump_info);

TreeErr_t TreeGraphDump       (const Tree_t* tree,
                               const char* image_name,
                               const char* dot_dir);

TreeErr_t TreeNodeDump        (const TreeNode_t* node, FILE* fp);
TreeErr_t TreeSingleNodeDump  (const TreeNode_t* node, FILE* fp);
int       MakeTreeEdges       (const TreeNode_t* node, FILE* fp);

int MakeTreeDefaultNode(
    const TreeNode_t* node,
    const char* color,
    const char* fillcolor,
    const char* fontcolor,
    const char* shape,
    FILE* fp);

int SetDirectories(char* log_filename, char* log_dir);

//——————————————————————————————————————————————————————————————————————————————————————————

int MakeNode(
    const char* name,
    const char* label,
    const char* color,
    const char* fillcolor,
    const char* fontcolor,
    const char* shape,
    FILE* fp);

int MakeEdge(
    const char* node1,
    const char* node2,
    const char* color,
    const char* constraint,
    const char* dir,
    const char* style,
    const char* arrowhead,
    const char* arrowtail,
    FILE*       fp);

int PrintArg(
    const char* arg_name,
    const char* arg_value,
    int*        is_first_arg,
    FILE*       fp);

//——————————————————————————————————————————————————————————————————————————————————————————

const int MAX_NODE_NAME_LEN = 32;
const int MAX_LABEL_LEN     = 256;

//——————————————————————————————————————————————————————————————————————————————————————————

#endif /* TREE_GRAPH_H */
