#ifndef TREE_GRAPH_H
#define TREE_GRAPH_H

//——————————————————————————————————————————————————————————————————————————————————————————

#include "tree.h"

//——————————————————————————————————————————————————————————————————————————————————————————

TreeErr_t TreeDump            (const Tree_t*     tree);
TreeErr_t TreeNodeDump        (const TreeNode_t* node, FILE* fp);
TreeErr_t TreeSingleNodeDump  (const TreeNode_t* node, FILE* fp);
int       MakeTreeEdges       (const TreeNode_t* node, FILE* fp);
int       MakeTreeDefaultNode (const TreeNode_t* node, FILE* fp);

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
