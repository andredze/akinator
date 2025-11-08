#include "tree.h"

//------------------------------------------------------------------------------------------

TreeErr_t TreeLoopPrint(Tree_t* tree)
{
    if (tree->dummy->right == NULL)
    {
        return TREE_SUCCESS;
    }

    Stack_t stack = {};

    if (StackCtor(&stack, STACK_MIN_CAPACITY))
    {
        return TREE_STACK_ERROR;
    }

    size_t i = 0;
    TreeCallsCtx_t node_ctx = {tree->dummy, '\0'};

    while (node_ctx.node != tree->dummy || i == 0)
    {
        if (i > 100)
        {
            PRINTERR("Iterations exceeded max (i = %zu, size = %zu)", i, tree->size);
            StackDtor(&stack);
            return TREE_LOOP;
        }

        DPRINTF("node = %p, data = " TREE_SPEC ", letter = %c;\n", node_ctx.node, node_ctx.node->data, node_ctx.letter);

        if (node_ctx.letter == '\0')
        {
            if (node_ctx.node->left != NULL)
            {
                StackPush(&stack, {node_ctx.node,  'L'});
                node_ctx.node = node_ctx.node->left;
                continue;
            }

            if (node_ctx.node != tree->dummy)
            {
                printf(" " TREE_SPEC " ", node_ctx.node->data);
            }

            if (node_ctx.node->right != NULL)
            {
                StackPush(&stack, {node_ctx.node, 'R'});
                node_ctx.node = node_ctx.node->right;
            }
            else
            {
                DPRINTF("bef stackpop: node = %p, data = " TREE_SPEC ", letter = %c;\n", node_ctx.node, node_ctx.node->data, node_ctx.letter);
                StackPop(&stack, &node_ctx);
                DPRINTF("aft stackpop: node = %p, data = " TREE_SPEC ", letter = %c;\n", node_ctx.node, node_ctx.node->data, node_ctx.letter);
            }
        }
        else if (node_ctx.letter == 'L')
        {
            printf(" " TREE_SPEC " ", node_ctx.node->data);

            if (node_ctx.node->right != NULL)
            {
                StackPush(&stack, {node_ctx.node, 'R'});
                node_ctx.node = node_ctx.node->right;
                node_ctx.letter = '\0';
            }
            else
            {
                DPRINTF("bef stackpop: node = %p, data = " TREE_SPEC ", letter = %c;\n", node_ctx.node, node_ctx.node->data, node_ctx.letter);
                StackPop(&stack, &node_ctx);
                DPRINTF("aft stackpop: node = %p, data = " TREE_SPEC ", letter = %c;\n", node_ctx.node, node_ctx.node->data, node_ctx.letter);
            }
        }
        else
        {
            DPRINTF("bef stackpop: node = %p, data = " TREE_SPEC ", letter = %c;\n", node_ctx.node, node_ctx.node->data, node_ctx.letter);
            StackPop(&stack, &node_ctx);
            DPRINTF("aft stackpop: node = %p, data = " TREE_SPEC ", letter = %c;\n", node_ctx.node, node_ctx.node->data, node_ctx.letter);
        }
        i++;
    }

    StackDtor(&stack);

    return TREE_SUCCESS;
}

//------------------------------------------------------------------------------------------

TreeErr_t TreeCheck(Tree_t*     tree,
                    const char* func,
                    const char* file,
                    int         line,
                    void*       arg)
{
    assert(func    != NULL);
    assert(file    != NULL);

    TreeErr_t verify_status = TREE_SUCCESS;

    if ((verify_status = TreeVerify(tree)))
    {
        PRINTERR("%s (TreeVerify not passed! Check \"tree.html\")", TREE_STR_ERRORS[verify_status]);

        TreeDumpInfo_t dump_info = {verify_status, "error_dump", func, file, line, arg};

        if (TreeDump(tree, &dump_info))
        {
            return TREE_DUMP_ERROR;
        }
    }

    return verify_status;
}

//------------------------------------------------------------------------------------------

TreeErr_t TreeSetValuesToArray(Tree_t* tree, int* array)
{
    assert(tree  != NULL);
    assert(array != NULL);

    size_t i = 0;

    TreeErr_t error = TREE_SUCCESS;

    if (tree->dummy->right == NULL)
    {
        return TREE_SUCCESS;
    }

    if ((error = TreeSetValue(tree->dummy->right, array, &i)))
    {
        return error;
    }

    return TREE_SUCCESS;
}

//------------------------------------------------------------------------------------------

TreeErr_t TreeSetValue(const TreeNode_t* node, int* array, size_t* i)
{
    assert(array != NULL);
    assert(i     != NULL);

    if (node == NULL)
    {
        return TREE_NULL_NODE;
    }

    TreeErr_t error = TREE_SUCCESS;

    if (node->left != NULL)
    {
        if ((error = TreeSetValue(node->left, array, i)))
        {
            return error;
        }
    }

    DPRINTF("array[%zu] = " TREE_SPEC ";\n", *i, node->data);
    array[(*i)++] = node->data;

    if (node->right != NULL)
    {
        if ((error = TreeSetValue(node->right, array, i)))
        {
            return error;
        }
    }

    return TREE_SUCCESS;
}

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

    TREE_CALL_DUMP(tree, "DUMP AFTER CTOR ", 0);

    DPRINTF("CTOR END\n");

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
    // DEBUG_TREE_CHECK();

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

    TREE_CALL_DUMP(tree, "DUMP AFTER INSERT DATA TO PTR = ", new_node);

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

    node->data  = TREE_POISON;
    node->left  = NULL;
    node->right = NULL;

    free(node);

    return TREE_SUCCESS;
}

//------------------------------------------------------------------------------------------

TreeErr_t TreeNodeVerify(Tree_t* tree, TreeNode_t* node, size_t* calls_count)
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

TreeErr_t TreeVerify(Tree_t* tree)
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
