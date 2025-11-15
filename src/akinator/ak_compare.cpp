#include "akinator.h"

//------------------------------------------------------------------------------------------

TreeErr_t AkinatorExecuteCompare(AkinatorCtx_t* ak_ctx)
{
    assert(ak_ctx != NULL);

    TreeErr_t error = TREE_SUCCESS;

    Speak(BLUE, "—равнение двух слов:\n");
    SpeakFlush();

    cprintf(NULL, " ¬ведите первое слово: ");

    TreeNode_t* node1  = NULL;
    TreeNode_t* node2  = NULL;
    Stack_t     stack1 = {};
    Stack_t     stack2 = {};

    if (GetPathStack(&ak_ctx->tree, &node1, &stack1))
    {
        return TREE_SUCCESS;
    }

    cprintf(NULL, " ¬ведите второе слово: ");

    if (GetPathStack(&ak_ctx->tree, &node2, &stack2))
    {
        StackDtor(&stack1);
        return TREE_SUCCESS;
    }

    printf("\n");

    if ((error = AkinatorCompareNodes(&stack1, &stack2, node1, node2)))
    {
        return error;
    }

    return TREE_SUCCESS;
}

//------------------------------------------------------------------------------------------

TreeErr_t GetPathStack(Tree_t* tree, TreeNode_t** node, Stack_t* stack)
{
    assert(node  != NULL);
    assert(stack != NULL);

    DEBUG_TREE_CHECK(tree, "ERROR BEFORE AKINATOR COMPARE WORDS");

    if (StackCtor(stack, STACK_MIN_CAPACITY))
    {
        return TREE_STACK_ERR;
    }

    TreeErr_t error = TREE_SUCCESS;

    if ((error = FillPathStack(tree, node, stack)))
    {
        return error;
    }

    return TREE_SUCCESS;
}

//------------------------------------------------------------------------------------------

TreeErr_t FillPathStack(Tree_t* tree, TreeNode_t** node, Stack_t* stack)
{
    char word[MAX_INPUT_LEN] = {};

    while (scanf("%1023[^\n]", word) != 1)
    {
        CleanBuffer();
        Speak(RED, "ќшибка ввода, введите еще раз\n");
        SpeakFlush();
    }

    CleanBuffer();

    *node = TreeSearch(tree->dummy->right, word, stack);

    if (*node == NULL)
    {
        Speak(RED, "—лова %s нет в базе!\n", word);
        SpeakFlush();
        StackDtor(stack);
        return TREE_INVALID_INPUT;
    }

    return TREE_SUCCESS;
}

//------------------------------------------------------------------------------------------

TreeErr_t AkinatorCompareNodes(Stack_t* stack1,
                               Stack_t* stack2,
                               const TreeNode_t* node1,
                               const TreeNode_t* node2)
{
    assert(node1 != NULL);
    assert(node2 != NULL);

    TreeErr_t error = TREE_SUCCESS;

    size_t pos1 = 0;
    size_t pos2 = 0;

    if ((error = FindFirstDifference(stack1, stack2, &pos1, &pos2)))
    {
        StackDtor(stack1);
        StackDtor(stack2);
        return error;
    }

    PrintComparison(stack1, stack2, node1, node2, pos1, pos2);

    StackDtor(stack1);
    StackDtor(stack2);

    return TREE_SUCCESS;
}

//------------------------------------------------------------------------------------------

void PrintComparison(Stack_t* stack1,
                     Stack_t* stack2,
                     const TreeNode_t* node1,
                     const TreeNode_t* node2,
                     size_t pos1,
                     size_t pos2)
{
    assert(stack1 != NULL);
    assert(stack2 != NULL);
    assert(node1  != NULL);
    assert(node2  != NULL);

    Speak(BLUE, "—равнение %s и %s:\n", node1->data, node2->data);

    if (pos1 != stack1->size - 1)
    {
        Speak(NULL, " %s, как и %s ", node1->data, node2->data);
        PrintStackWordPath(stack1, stack1->size - 1, pos1 + 1);
    }
    else
    {
        Speak(NULL, " %s и %s не имеют ничего общего\n", node1->data, node2->data);
    }

    Speak(NULL, " но %s ", node1->data);
    PrintStackWordPath(stack1, pos1, 0);

    Speak(NULL, " а %s ", node2->data);
    PrintStackWordPath(stack2, pos2, 0);

    printf("\n");

    SpeakFlush();
}

//------------------------------------------------------------------------------------------

TreeErr_t FindFirstDifference(Stack_t* stack1,   Stack_t* stack2,
                              size_t*  pos1_ptr, size_t*  pos2_ptr)
{
    assert(stack1   != NULL);
    assert(stack2   != NULL);
    assert(pos1_ptr != NULL);
    assert(pos2_ptr != NULL);

    size_t pos1 = 0;
    size_t pos2 = 0;

    if (StackGetSize(stack1, &pos1))
        return TREE_STACK_ERR;

    if (StackGetSize(stack2, &pos2))
        return TREE_STACK_ERR;

    TreeStep_t step1 = {};
    TreeStep_t step2 = {};

    // DPRINTF("pos1 = %zu; pos2 = %zu\n", pos1, pos2);

    while (pos1 != 0 && pos2 != 0)
    {
        if (StackGetElement(stack1, pos1, &step1))
            return TREE_STACK_ERR;

        if (StackGetElement(stack2, pos2, &step2))
            return TREE_STACK_ERR;

        if (!(TreeStepsEqual(&step1, &step2)))
        {
            DPRINTF("unequal words = %s and %s\n", step1.node->data, step2.node->data);
            break;
        }

        pos1--;
        pos2--;
    }

    // DPRINTF("pos1 = %zu; pos2 = %zu\n", pos1, pos2);

    *pos1_ptr = pos1;
    *pos2_ptr = pos2;

    return TREE_SUCCESS;
}

//------------------------------------------------------------------------------------------

int TreeStepsEqual(TreeStep_t* step1, TreeStep_t* step2)
{
    assert(step1 != NULL);
    assert(step2 != NULL);

    if (step1->connection != step2->connection)
    {
        return 0;
    }
    if (step1->node != step2->node)
    {
        return 0;
    }

    return 1;
}

//------------------------------------------------------------------------------------------

TreeNode_t* TreeSearch(TreeNode_t* node, const char* word, Stack_t* stack)
{
    if (node == NULL)
    {
        return NULL;
    }

    if (strcmp(node->data, word) == 0)
    {
        return node;
    }

    TreeNode_t* ret = NULL;

    if ((ret = TreeSearch(node->left, word, stack)) != NULL)
    {
        if (StackPush(stack, {node, 'y'}))
        {
            return NULL;
        }

        return ret;
    }
    if ((ret = TreeSearch(node->right, word, stack)) != NULL)
    {
        if (StackPush(stack, {node, 'n'}))
        {
            return NULL;
        }

        return ret;
    }

    return NULL;
}

//------------------------------------------------------------------------------------------
