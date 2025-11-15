#include "akinator.h"

//------------------------------------------------------------------------------------------

TreeErr_t AkinatorExecuteDescribe(AkinatorCtx_t* ak_ctx)
{
    assert(ak_ctx != NULL);

    TreeErr_t error = TREE_SUCCESS;

    char word[MAX_INPUT_LEN] = {};

    Speak(BLUE, "Определение слова:\n");

    cprintf(NULL, " Введите слово: ");

    SpeakFlush();

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
        Speak(RED, "Такого слова нет!\n");
        SpeakFlush();
        StackDtor(&stack);
        return TREE_SUCCESS;
    }

    if ((error = GetStackWordPath(tree, node, &stack)))
    {
        StackDtor(&stack);
        return error;
    }

    Speak(BLUE, "Определение %s:\n", word);
    SpeakFlush();

    Speak(NULL, " %s ", word);

    if ((error = PrintStackWordPath(&stack)))
    {
        SpeakFlush();
        StackDtor(&stack);
        return error;
    }

    SpeakFlush();

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
        Speak(NULL, "\n");
        return;
    }
    else if (stack_size < 2)
    {
        PrintCondition(step);
        Speak(NULL, " и ");
        return;
    }
    else
    {
        PrintCondition(step);
        Speak(NULL, ", ");
    }
}

//------------------------------------------------------------------------------------------

void PrintCondition(TreeStep_t* step)
{
    if (step->connection == 'y')
    {
        Speak(NULL, "%s", step->node->data);
    }
    else if (step->connection == 'n')
    {
        Speak(NULL, "не %s", step->node->data);
    }
}

//------------------------------------------------------------------------------------------
