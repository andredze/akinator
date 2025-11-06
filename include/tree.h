#ifndef TREE_H
#define TREE_H

//——————————————————————————————————————————————————————————————————————————————————————————

#include "treeTypes.h"
#include "treeGraph.h"

//——————————————————————————————————————————————————————————————————————————————————————————

TreeErr_t TreeCtor           (Tree_t*     tree);
TreeErr_t TreeNodeCtor       (Tree_t*     tree, TreeElem_t data, TreeNode_t** new_node);
TreeErr_t TreeDtor           (Tree_t*     tree);
TreeErr_t TreeInsert         (Tree_t*     tree, TreeElem_t data);
TreeErr_t TreeSingleNodeDtor (TreeNode_t* node);
TreeErr_t TreeNodeDtor       (TreeNode_t* node);
TreeErr_t TreeNodeVerify     (TreeNode_t* node);

//——————————————————————————————————————————————————————————————————————————————————————————

#endif /* TREE_H */
