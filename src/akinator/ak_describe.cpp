#include "akinator.h"

//------------------------------------------------------------------------------------------

TreeErr_t AkinatorExecuteDescribe(AkinatorCtx_t* ak_ctx)
{
    assert(ak_ctx != NULL);

    TreeErr_t   error = TREE_SUCCESS;
    TreeNode_t* node  = NULL;
    Stack_t     stack = {};

    Speak(BLUE, "Определение слова:\n");
    SpeakFlush();

    cprintf(NULL, " Введите слово: ");

    if ((error = GetPathStack(&ak_ctx->tree, &node, &stack)))
    {
        return error;
    }

    printf("\n");

    if ((error = AkinatorDescribeWord(&stack, node)))
    {
        return error;
    }

    return TREE_SUCCESS;
}

//------------------------------------------------------------------------------------------

TreeErr_t AkinatorDescribeWord(Stack_t* stack, const TreeNode_t* node)
{
    assert(stack != NULL);
    assert(node  != NULL);

    TreeErr_t error = TREE_SUCCESS;

    Speak(BLUE, "Определение %s:\n", node->data);
    SpeakFlush();

    Speak(NULL, " %s ", node->data);

    if ((error = PrintStackWordPath(stack, stack->size - 1, 0)))
    {
        SpeakFlush();
        StackDtor(stack);
        return error;
    }

    SpeakFlush();

    printf("\n");

    if (StackDtor(stack))
    {
        return TREE_STACK_ERR;
    }

    return TREE_SUCCESS;
}

//------------------------------------------------------------------------------------------

TreeErr_t PrintStackWordPath(Stack_t* stack, size_t start_pos, size_t end_pos)
{
    assert(stack != NULL);

    size_t     cur_pos  = start_pos;
    TreeStep_t cur_step = {};

    while (cur_pos >= end_pos)
    {
        // DPRINTF("cur_pos = %zu; end_pos = %zu;\n", cur_pos, end_pos);

        if (StackGetElement(stack, cur_pos, &cur_step))
        {
            PRINTERR("Stack pop failed");
            return TREE_STACK_ERR;
        }

        PrintConditionFormatted(&cur_step, cur_pos - end_pos);

        if (cur_pos == 0)
        {
            break;
        }

        cur_pos--;
    }

    return TREE_SUCCESS;
}

//------------------------------------------------------------------------------------------

void PrintConditionFormatted(TreeStep_t* step, size_t pos)
{
    if (pos > STACK_SIZE_LIMIT)
    {
        return;
    }
    else if (pos < 1)
    {
        PrintCondition(step);
        Speak(NULL, "\n");
        return;
    }
    else if (pos < 2)
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
