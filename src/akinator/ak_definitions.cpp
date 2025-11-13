#include "akinator.h"

//------------------------------------------------------------------------------------------

TreeErr_t AkinatorExecuteCompare(AkinatorCtx_t* ak_ctx)
{
    assert(ak_ctx != NULL);

    TreeErr_t error = TREE_SUCCESS;

    char word1[MAX_INPUT_LEN] = {};
    char word2[MAX_INPUT_LEN] = {};

    printf(BLUE "Сравнение двух слов:\n" RESET_CLR);

    printf(" Введите первое слово: ");

    if (scanf("%1023[^\n]", word1) != 1)
    {
        PRINTERR("Scanf failed");
        return TREE_INVALID_INPUT;
    }

    CleanBuffer();

    printf(" Введите второе слово: ");

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
        printf(BLUE "Слова %s нет в базе!\n" RESET_CLR, word1);
        return TREE_SUCCESS;
    }

    TreeNode_t* node2 = TreeSearch(tree->dummy->right, word2);
    if (node2 == NULL)
    {
        printf(BLUE "Слова %s нет в базе!\n" RESET_CLR, word2);
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

    printf(BLUE "Сравнение %s и %s:\n" RESET_CLR, word1, word2);

    if (common_stack->size > 0)
    {
        printf(" %s, как и %s ", word1, word2);
        PrintStackWordPath(common_stack);
    }
    else
    {
        printf(" %s и %s не имеют ничего общего\n", word1, word2);
    }

    printf(" но %s ", word1);

    PrintStackWordPath(stack1);

    printf(" a %s ", word2);

    PrintStackWordPath(stack2);

    printf("\n");
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
    if ((error = GetCommonConditionsStack(stack1, stack2, common_stack)))
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

TreeErr_t GetCommonConditionsStack(Stack_t* stack1, Stack_t* stack2, Stack_t* common_stack)
{
    assert(common_stack != NULL);
    assert(stack1       != NULL);
    assert(stack2       != NULL);

    Stack_t reversed_common = {};

    if (StackCtor(&reversed_common, STACK_MIN_CAPACITY))
    {
        return TREE_STACK_ERR;
    }

    TreeStep_t step1 = {};
    TreeStep_t step2 = {};

    while (1)
    {
        if (StackPop(stack1, &step1))
            return TREE_STACK_ERR;

        if (StackPop(stack2, &step2))
            return TREE_STACK_ERR;

        if (TreeStepsEqual(&step1, &step2))
        {
            if (StackPush(&reversed_common, step1))
                return TREE_STACK_ERR;
        }
        else
        {
            if (StackPush(stack1, step1))
                return TREE_STACK_ERR;

            if (StackPush(stack2, step2))
                return TREE_STACK_ERR;

            break;
        }
    }

    if (ReverseStack(&reversed_common, common_stack))
        return TREE_STACK_ERR;

    if (StackDtor(&reversed_common))
        return TREE_STACK_ERR;

    return TREE_SUCCESS;
}

//------------------------------------------------------------------------------------------

TreeErr_t ReverseStack(Stack_t* src_stack, Stack_t* dest_stack)
{
    assert(src_stack  != NULL);
    assert(dest_stack != NULL);

    item_t elem = POISON;

    while (src_stack->size > 0)
    {
        if (StackPop(src_stack, &elem))
            return TREE_STACK_ERR;

        if (StackPush(dest_stack, elem))
            return TREE_STACK_ERR;
    }

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

TreeErr_t AkinatorExecuteDescribe(AkinatorCtx_t* ak_ctx)
{
    assert(ak_ctx != NULL);

    TreeErr_t error = TREE_SUCCESS;

    char word[MAX_INPUT_LEN] = {};

    printf(BLUE "Определение слова:\n" RESET_CLR);

    printf(" Введите слово: ");

    if (scanf("%1023[^\n]", word) != 1)
    {
        PRINTERR("Scanf failed");
        return TREE_INVALID_INPUT;
    }

    CleanBuffer();

    printf("\n");

    if ((error = AkinatorDescribeWord(&ak_ctx->tree, word)))
    {
        return error;
    }

    return TREE_SUCCESS;
}

//------------------------------------------------------------------------------------------

TreeErr_t AkinatorDescribeWord(const Tree_t* tree, const char* word)
{
    assert(tree != NULL);
    assert(word != NULL);

    Stack_t stack = {};

    TreeErr_t error = TREE_SUCCESS;

    if (StackCtor(&stack, STACK_MIN_CAPACITY))
    {
        return TREE_STACK_ERR;
    }

    TreeNode_t* node = TreeSearch(tree->dummy->right, word);

    if (node == NULL)
    {
        printf(BLUE "Такого слова нет!\n" RESET_CLR);
        StackDtor(&stack);
        return TREE_SUCCESS;
    }

    if ((error = GetStackWordPath(tree, node, &stack)))
    {
        StackDtor(&stack);
        return error;
    }

    printf(BLUE "Определение %s:\n" RESET_CLR " %s ", word, word);

    if ((error = PrintStackWordPath(&stack)))
    {
        StackDtor(&stack);
        return error;
    }

    printf("\n");

    if (StackDtor(&stack))
    {
        return TREE_STACK_ERR;
    }

    return TREE_SUCCESS;
}

//------------------------------------------------------------------------------------------

TreeErr_t PrintStackWordPath(Stack_t* stack)
{
    assert(stack != NULL);

    while (0 < stack->size)
    {
        TreeStep_t cur_step = {};

        if (StackPop(stack, &cur_step))
        {
            PRINTERR("Stack pop failed");
            return TREE_STACK_ERR;
        }

        PrintConditionFormatted(&cur_step, stack->size);
    }

    return TREE_SUCCESS;
}

//------------------------------------------------------------------------------------------

TreeErr_t GetStackWordPath(const Tree_t* tree, TreeNode_t* node, Stack_t* stack)
{
    assert(node  != NULL);
    assert(stack != NULL);

    DEBUG_TREE_CHECK(tree, "ERROR DUMP BEFORE STACK GET WORD PATH");

    char connection = '\0';

    while (node != tree->dummy->right)
    {
        if (node->parent->right == node)
        {
            connection = 'n';
        }
        else
        {
            connection = 'y';
        }

        // DPRINTF("parent of %s: %s, %p, connection = %c;\n",
        //         node->data,
        //         node->parent->data,
        //         node->parent,
        //         connection);

        if (StackPush(stack, {node->parent, connection}))
        {
            return TREE_STACK_ERR;
        }

        node = node->parent;
    }

    return TREE_SUCCESS;
}

//------------------------------------------------------------------------------------------

void PrintConditionFormatted(TreeStep_t* step, size_t stack_size)
{
    if (stack_size > STACK_SIZE_LIMIT)
    {
        return;
    }
    else if (stack_size < 1)
    {
        PrintCondition(step);
        printf("\n");
        return;
    }
    else if (stack_size < 2)
    {
        PrintCondition(step);
        printf(" и ");
        return;
    }
    else
    {
        PrintCondition(step);
        printf(", ");
    }
}

//------------------------------------------------------------------------------------------

void PrintCondition(TreeStep_t* step)
{
    if (step->connection == 'y')
    {
        printf("%s", step->node->data);
    }
    else if (step->connection == 'n')
    {
        printf("не %s", step->node->data);
    }
}

//------------------------------------------------------------------------------------------
