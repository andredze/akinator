#include "tree.h"

//------------------------------------------------------------------------------------------

TreeErr_t TreeCtor(Tree_t* tree)
{
    assert(tree != NULL);

    TreeNode_t* dummy = NULL;

    TreeErr_t error = TREE_SUCCESS;

    if ((error = TreeNodeCtor(tree, 0, &dummy)))
    {
        return error;
    }

    tree->dummy = dummy;

    tree->size  = 0;

    return TREE_SUCCESS;
}

//------------------------------------------------------------------------------------------

TreeErr_t TreeNodeCtor(Tree_t* tree, TreeElem_t data, TreeNode_t** new_node)
{
    assert(tree     != NULL);
    assert(new_node != NULL);

    TreeNode_t* node = (TreeNode_t*) calloc (1, sizeof(TreeNode_t));

    if (node == NULL)
    {
        PRINTERR("Memory allocation for a new node failed");
        return TREE_CALLOC_ERROR;
    }

    node->data  = data;

    tree->size++;

    *new_node = node;

    return TREE_SUCCESS;
}

//------------------------------------------------------------------------------------------

TreeErr_t TreeInsert(Tree_t* tree, TreeElem_t data)
{
    assert(tree != NULL);

    TreeNode_t* node     = tree->dummy;
    TreeNode_t* new_node = NULL;

    TreeErr_t   error = TREE_SUCCESS;

    if ((error = TreeNodeCtor(tree, data, &new_node)))
    {
        return error;
    }

    while (node != NULL)
    {
        if (data <= node->data)
        {
            if (node->left == NULL)
            {
                node->left = new_node;
                break;
            }
            node = node->left;
        }
        else
        {
            if (node->right == NULL)
            {
                node->right = new_node;
                break;
            }
            node = node->right;
        }
    }

    return TREE_SUCCESS;
}

//------------------------------------------------------------------------------------------

TreeErr_t TreeDtor(Tree_t* tree)
{
    assert(tree != NULL);

    TreeErr_t error = TREE_SUCCESS;

    if ((error = TreeNodeDtor(tree->dummy)))
    {
        return error;
    }

    return TREE_SUCCESS;
}

//------------------------------------------------------------------------------------------

TreeErr_t TreeNodeDtor(TreeNode_t* node)
{
    assert(node != NULL);

    TreeErr_t error = TREE_SUCCESS;

    if (node->left != NULL)
    {
        if ((error = TreeNodeDtor(node->left)))
        {
            return error;
        }
    }

    if (node->right != NULL)
    {
        if ((error = TreeNodeDtor(node->right)))
        {
            return error;
        }
    }

    if ((error = TreeSingleNodeDtor(node)))
    {
        return error;
    }

    return TREE_SUCCESS;
}

//------------------------------------------------------------------------------------------

TreeErr_t TreeSingleNodeDtor(TreeNode_t* node)
{
    if (node == NULL)
    {
        return TREE_NULL_NODE;
    }

    node->data  = TREE_POISON;
    node->left  = NULL;
    node->right = NULL;

    free(node);

    return TREE_SUCCESS;
}

//------------------------------------------------------------------------------------------

TreeErr_t TreeNodeVerify(TreeNode_t* node)
{
    if (node == NULL)
    {
        return TREE_NULL_NODE;
    }

    return TREE_SUCCESS;
}

//------------------------------------------------------------------------------------------
