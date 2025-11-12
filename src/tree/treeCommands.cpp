#include <locale.h>
#include <langinfo.h>
#include "treeCommands.h"

//------------------------------------------------------------------------------------------

#ifdef TREE_DEBUG

TreeErr_t TreeCheck(const Tree_t* tree,
                    const char*   func,
                    const char*   file,
                    int           line,
                    const char*   fmt, ...)
{
    assert(func != NULL);
    assert(file != NULL);
    assert(fmt  != NULL);

    TreeErr_t verify_status = TREE_SUCCESS;

    if ((verify_status = TreeVerify(tree)))
    {
        PRINTERR("%s (TreeVerify not passed! Check \"tree.html\")", TREE_STR_ERRORS[verify_status]);

        TreeDumpInfo_t dump_info = {verify_status, func, file, line};

        va_list args = {};
        va_start(args, fmt);

        if (vTreeDump(tree, &dump_info, fmt, args))
        {
            return TREE_DUMP_ERROR;
        }

        va_end(args);
    }

    return verify_status;
}

#endif /* TREE_DEBUG */

//------------------------------------------------------------------------------------------

TreeErr_t TreeCtor(Tree_t* tree)
{
    assert(tree != NULL);

    TreeNode_t* dummy = NULL;
    TreeErr_t   error = TREE_SUCCESS;

    if ((error = TreeNodeCtor(tree, TREE_POISON, &dummy)))
    {
        return error;
    }

    tree->dummy = dummy;

    tree->size  = 0;

    DEBUG_TREE_CHECK(tree, "ERROR DUMP AFTER CTOR");
    // TREE_CALL_DUMP(tree, "DUMP AFTER CTOR");

    DPRINTF("> TreeCtor   END\n");

    return TREE_SUCCESS;
}

//------------------------------------------------------------------------------------------

TreeErr_t TreeNodeCtor(Tree_t* tree, TreeElem_t data, TreeNode_t** new_node)
{
    if (new_node == NULL)
    {
        PRINTERR("Null node_ptr given to insert to");
        return TREE_NULL_NODE;
    }

    TreeNode_t* node = (TreeNode_t*) calloc(1, sizeof(TreeNode_t));

    if (node == NULL)
    {
        PRINTERR("Memory allocation for a new node failed");
        return TREE_CALLOC_ERROR;
    }

    node->left  = NULL;
    node->right = NULL;
    node->data  = data;

    tree->size++;

    *new_node = node;

    return TREE_SUCCESS;
}

//------------------------------------------------------------------------------------------

TreeErr_t TreeInsert(Tree_t* tree, TreeElem_t data)
{
    DEBUG_TREE_CHECK(tree, "ERROR DUMP TREE BEFORE INSERT data = " TREE_SPEC, data);

    TreeNode_t* node     = tree->dummy;
    TreeNode_t* new_node = NULL;

    TreeErr_t   error    = TREE_SUCCESS;

    if ((error = TreeNodeCtor(tree, data, &new_node)))
    {
        return error;
    }

    char answer[1000] = {};

    while (node != NULL)
    {
        printf("Does your character %s? (y/n): ", node->data);
        scanf("%s", answer);

        if (strcmp(answer, "y") == 0)
        {
            if (node->left == NULL)
            {
                node->left = new_node;
                break;
            }
            node = node->left;
        }
        else if (strcmp(answer, "n") == 0)
        {
            if (node->right == NULL)
            {
                node->right = new_node;
                break;
            }
            node = node->right;
        }
    }

    DEBUG_TREE_CHECK(tree, "ERROR DUMP TREE AFTER INSERT data = " TREE_SPEC " to %p", data, new_node);

    TREE_CALL_DUMP(tree, "DUMP TREE AFTER INSERT data = " TREE_SPEC " to %p", data, new_node);

    return TREE_SUCCESS;
}

//------------------------------------------------------------------------------------------

TreeErr_t TreeDtor(Tree_t* tree)
{
    assert(tree != NULL);

    free(tree->buffer);

    TreeErr_t error = TREE_SUCCESS;

    if ((error = TreeNodeDtor(tree->dummy)))
    {
        return error;
    }

    return TREE_SUCCESS;
}

//------------------------------------------------------------------------------------------

TreeErr_t TreeLeftSubtreeDtor(TreeNode_t* node)
{
    assert(node != NULL);

    return TreeSubtreeDtor(&node->left);
}

//------------------------------------------------------------------------------------------

TreeErr_t TreeRightSubtreeDtor(TreeNode_t* node)
{
    assert(node != NULL);

    return TreeSubtreeDtor(&node->right);
}

//------------------------------------------------------------------------------------------

TreeErr_t TreeSubtreeDtor(TreeNode_t** node_ptr)
{
    assert(node_ptr != NULL);

    if (*node_ptr == NULL)
    {
        return TREE_SUCCESS;
    }

    TreeErr_t error = TREE_SUCCESS;

    if ((error = TreeNodeDtor(*node_ptr)))
    {
        return error;
    }

    *node_ptr = NULL;

    return TREE_SUCCESS;
}

//------------------------------------------------------------------------------------------

TreeErr_t TreeNodeDtor(TreeNode_t* node)
{
    if (node == NULL)
    {
        return TREE_NULL_NODE;
    }

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

    if (node->dynamic_memory)
    {
        free(node->data);
    }

    node->data  = TREE_POISON;
    node->left  = NULL;
    node->right = NULL;

    free(node);

    return TREE_SUCCESS;
}

//------------------------------------------------------------------------------------------

TreeErr_t TreeNodeVerify(const Tree_t* tree, TreeNode_t* node, size_t* calls_count)
{
    assert(calls_count != NULL);
    assert(tree        != NULL);

    if (*calls_count > tree->size)
    {
        return TREE_LOOP;
    }

    if (node == NULL)
    {
        return TREE_NULL_NODE;
    }

    (*calls_count)++;

    TreeErr_t error = TREE_SUCCESS;

    if (node->left != NULL)
    {
        if ((error = TreeNodeVerify(tree, node->left, calls_count)))
        {
            return error;
        }
    }

    if (node->right != NULL)
    {
        if ((error = TreeNodeVerify(tree, node->right, calls_count)))
        {
            return error;
        }
    }

    return TREE_SUCCESS;
}

//------------------------------------------------------------------------------------------

TreeErr_t TreeVerify(const Tree_t* tree)
{
    if (tree == NULL)
    {
        return TREE_NULL;
    }
    if (tree->dummy == NULL)
    {
        return TREE_NULL_DUMMY;
    }
    if (tree->size > TREE_MAX_SIZE)
    {
        return TREE_SIZE_EXCEEDS_MAX;
    }

    size_t calls_count = 0;
    TreeErr_t error = TREE_SUCCESS;

    if ((error = TreeNodeVerify(tree, tree->dummy, &calls_count)))
    {
        return error;
    }

    return TREE_SUCCESS;
}

//------------------------------------------------------------------------------------------
