#include "akinator.h"

//------------------------------------------------------------------------------------------

TreeErr_t AkinatorExecuteCompare(AkinatorCtx_t* ak_ctx)
{
    assert(ak_ctx != NULL);

    TreeErr_t error = TREE_SUCCESS;

    char word1[MAX_INPUT_LEN] = {};
    char word2[MAX_INPUT_LEN] = {};

    Speak(BLUE, "—равнение двух слов:\n");

    cprintf(NULL, " ¬ведите первое слово: ");

    SpeakFlush();

    if (scanf("%1023[^\n]", word1) != 1)
    {
        PRINTERR("Scanf failed");
        return TREE_INVALID_INPUT;
    }

    CleanBuffer();

    cprintf(NULL, " ¬ведите второе слово: ");

    SpeakFlush();

    if (scanf("%1023[^\n]", word2) != 1)
    {
        PRINTERR("Scanf failed");
        return TREE_INVALID_INPUT;
    }

    CleanBuffer();

    printf("\n");

    if ((error = AkinatorCompareWords(&ak_ctx->tree, word1, word2)))
    {
        return error;
    }

    return TREE_SUCCESS;
}

//------------------------------------------------------------------------------------------

TreeErr_t AkinatorCompareWords(Tree_t* tree, const char* word1, const char* word2)
{
    assert(word1 != NULL);
    assert(word2 != NULL);

    DEBUG_TREE_CHECK(tree, "ERROR BEFORE AKINATOR COMPARE WORDS");

    TreeErr_t error = TREE_SUCCESS;

    Stack_t stack1       = {};
    Stack_t stack2       = {};
    Stack_t common_stack = {};

    TreeNode_t* node1 = TreeSearch(tree->dummy->right, word1);
    if (node1 == NULL)
    {
        Speak(RED, "—лова %s нет в базе!\n", word1);
        SpeakFlush();
        return TREE_SUCCESS;
    }

    TreeNode_t* node2 = TreeSearch(tree->dummy->right, word2);
    if (node2 == NULL)
    {
        Speak(RED, "—лова %s нет в базе!\n", word2);
        SpeakFlush();
        return TREE_SUCCESS;
    }

    if ((error = GetComparisonStacks(&stack1, &stack2, &common_stack, node1, node2, tree)))
    {
        return error;
    }

    PrintComparison(word1, word2, &stack1, &stack2, &common_stack);

    DestroyComparisonStacks(&stack1, &stack2, &common_stack);

    return TREE_SUCCESS;
}

//------------------------------------------------------------------------------------------

void PrintComparison(const char* word1, const char* word2,
                     Stack_t* stack1, Stack_t* stack2, Stack_t* common_stack)
{
    assert(word1        != NULL);
    assert(word2        != NULL);
    assert(stack1       != NULL);
    assert(stack2       != NULL);
    assert(common_stack != NULL);

    Speak(BLUE, "—равнение %s и %s:\n", word1, word2);

    if (common_stack->size > 0)
    {
        Speak(NULL, " %s, как и %s ", word1, word2);
        PrintStackWordPath(common_stack);
    }
    else
    {
        Speak(NULL, " %s и %s не имеют ничего общего\n", word1, word2);
    }
    if (stack1->size > 0)
    {
        Speak(NULL, " но %s ", word1);
        PrintStackWordPath(stack1);
    }
    else
    {
        Speak(NULL, " и %s не имеет отличительных свойств", word1);
    }
    if (stack2->size > 0)
    {
        Speak(NULL, " а %s ", word2);
        PrintStackWordPath(stack2);
    }
    else
    {
        Speak(NULL, " и %s не имеет отличительных свойств", word2);
    }

    printf("\n");

    SpeakFlush();
}

//------------------------------------------------------------------------------------------

TreeErr_t GetComparisonStacks(Stack_t* stack1, Stack_t* stack2, Stack_t* common_stack,
                              TreeNode_t* node1, TreeNode_t* node2, Tree_t* tree)
{
    assert(stack1       != NULL);
    assert(stack2       != NULL);
    assert(common_stack != NULL);

    TreeErr_t error = TREE_SUCCESS;

    if (ConstructComparisonStacks(stack1, stack2, common_stack))
    {
        return TREE_STACK_ERR;
    }
    if ((error = GetStackWordPath(tree, node1, stack1)))
    {
        DestroyComparisonStacks(stack1, stack2, common_stack);
        return error;
    }
    if ((error = GetStackWordPath(tree, node2, stack2)))
    {
        DestroyComparisonStacks(stack1, stack2, common_stack);
        return error;
    }
    if ((error = GetCommonConditionsStack(tree, stack1, stack2, common_stack)))
    {
        DestroyComparisonStacks(stack1, stack2, common_stack);
        return error;
    }

    return TREE_SUCCESS;
}

//------------------------------------------------------------------------------------------

TreeErr_t ConstructComparisonStacks(Stack_t* stack1, Stack_t* stack2, Stack_t* common_stack)
{
    assert(stack1       != NULL);
    assert(stack2       != NULL);
    assert(common_stack != NULL);

    if (StackCtor(stack1, STACK_MIN_CAPACITY))
    {
        return TREE_STACK_ERR;
    }
    if (StackCtor(stack2, STACK_MIN_CAPACITY))
    {
        StackDtor(stack1);
        return TREE_STACK_ERR;
    }
    if (StackCtor(common_stack, STACK_MIN_CAPACITY))
    {
        StackDtor(stack1);
        StackDtor(stack2);
        return TREE_STACK_ERR;
    }

    return TREE_SUCCESS;
}

//------------------------------------------------------------------------------------------

void DestroyComparisonStacks(Stack_t* stack1, Stack_t* stack2, Stack_t* common_stack)
{
    StackDtor(common_stack);
    StackDtor(stack1);
    StackDtor(stack2);
}

//------------------------------------------------------------------------------------------

TreeErr_t GetCommonConditionsStack(const Tree_t* tree, Stack_t* stack1, Stack_t* stack2, Stack_t* common_stack)
{
    assert(common_stack != NULL);
    assert(stack1       != NULL);
    assert(stack2       != NULL);

    TreeStep_t step1           = {};
    TreeStep_t step2           = {};
    TreeStep_t first_diff_step = {};

    while (1)
    {
        if (StackPop(stack1, &step1))
            return TREE_STACK_ERR;

        if (StackPop(stack2, &step2))
            return TREE_STACK_ERR;

        if (!(TreeStepsEqual(&step1, &step2)))
        {
            first_diff_step = step1;

            if (StackPush(stack1, step1))
                return TREE_STACK_ERR;

            if (StackPush(stack2, step2))
                return TREE_STACK_ERR;

            break;
        }
    }

    if (GetStackWordPath(tree, first_diff_step.node, common_stack))
        return TREE_STACK_ERR;

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

TreeNode_t* TreeSearch(TreeNode_t* node, const char* word)
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

    if ((ret = TreeSearch(node->left, word)) != NULL)
    {
        return ret;
    }
    if ((ret = TreeSearch(node->right, word)) != NULL)
    {
        return ret;
    }

    return NULL;
}

//------------------------------------------------------------------------------------------
