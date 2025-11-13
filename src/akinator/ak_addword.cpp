#include "akinator.h"

//------------------------------------------------------------------------------------------

TreeErr_t AkinatorAddWord(Tree_t* tree, TreeNode_t* guess_node)
{
    assert(guess_node != NULL);
    assert(tree       != NULL);

    TreeErr_t err = TREE_SUCCESS;

    char* new_word_data = NULL;
    char* condition_data  = NULL;

    if ((err = AkinatorGetNewWord(&new_word_data)))
    {
        return err;
    }

    int re_input_condition = 1;

    while (re_input_condition)
    {
        if ((err = AkinatorGetCondition(&condition_data, guess_node->data, new_word_data)))
        {
            return err;
        }
        re_input_condition = ConditionHasNegatives(condition_data);
    }

    TreeNode_t* new_word_node    = NULL;
    TreeNode_t* new_guessed_node = NULL;

    if ((err = TreeNodeCtor(tree, new_word_data, &new_word_node, guess_node)))
    {
        return err;
    }
    if ((err = TreeNodeCtor(tree, guess_node->data, &new_guessed_node, guess_node)))
    {
        return err;
    }

    new_word_node->dynamic_memory = 1;
    guess_node->dynamic_memory = 1;

    guess_node->data  = condition_data;
    guess_node->right = new_guessed_node;
    guess_node->left  = new_word_node;

    DEBUG_TREE_CHECK(tree, "ERROR DUMP AKINATOR ADD WORD");
    TREE_CALL_DUMP  (tree, "DUMP AFTER AKINATOR ADD WORD");

    return TREE_SUCCESS;
}

//------------------------------------------------------------------------------------------

TreeErr_t AkinatorGetNewWord(char** new_word_data)
{
    assert(new_word_data != NULL);

    char new_word_buf[MAX_INPUT_LEN] = "";
    int  new_word_len = 0;

    printf("Кто это был? Это был ");

    if (scanf("%1023[^\n]%n", new_word_buf, &new_word_len) != 1)
    {
        PRINTERR("Input error");
        return TREE_INVALID_INPUT;
    }

    CleanBuffer();

    char* new_word = strdup(new_word_buf);

    if (new_word == NULL)
    {
        return TREE_CALLOC_ERROR;
    }

    *new_word_data = new_word;

    return TREE_SUCCESS;
}

//------------------------------------------------------------------------------------------

TreeErr_t AkinatorGetCondition(char** condition_data, const char* guess_word, const char* new_word)
{
    assert(condition_data != NULL);
    assert(guess_word   != NULL);
    assert(new_word     != NULL);

    char condition_buf[MAX_INPUT_LEN] = "";
    int  condition_len = 0;

    printf("В отличие от %s %s ", guess_word, new_word);

    if (scanf("%1023[^\n]%n", condition_buf, &condition_len) != 1)
    {
        PRINTERR("Input error");
        return TREE_INVALID_INPUT;
    }

    CleanBuffer();

    char* condition = strdup(condition_buf);

    if (condition == NULL)
    {
        return TREE_CALLOC_ERROR;
    }

    *condition_data = condition;

    return TREE_SUCCESS;
}

//------------------------------------------------------------------------------------------

int ConditionHasNegatives(char* condition)
{
    assert(condition != NULL);

    if (strstr(condition, "не ") != NULL || strstr(condition, " не ") != NULL)
    {
        free(condition);
        return 1;
    }

    return 0;
}

//------------------------------------------------------------------------------------------
