#include "akinator.h"

//------------------------------------------------------------------------------------------

TreeErr_t AkinatorAddWord(Tree_t* tree, TreeNode_t* guess_node)
{
    assert(guess_node != NULL);
    assert(tree       != NULL);

    TreeErr_t err = TREE_SUCCESS;

    char* new_word_data = NULL;
    char* feature_data  = NULL;

    if ((err = AkinatorGetNewWord(&new_word_data)))
    {
        return err;
    }

    int re_input_feature = 1;

    while (re_input_feature)
    {
        if ((err = AkinatorGetFeature(&feature_data, guess_node->data, new_word_data)))
        {
            return err;
        }
        re_input_feature = FeatureHasNegatives(feature_data);
    }

    TreeNode_t* new_word_node    = NULL;
    TreeNode_t* new_guessed_node = NULL;

    if ((err = TreeNodeCtor(tree, new_word_data, &new_word_node)))
    {
        return err;
    }
    if ((err = TreeNodeCtor(tree, guess_node->data, &new_guessed_node)))
    {
        return err;
    }

    new_word_node->dynamic_memory = 1;
    guess_node->dynamic_memory = 1;

    guess_node->data  = feature_data;
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

TreeErr_t AkinatorGetFeature(char** feature_data, const char* guess_word, const char* new_word)
{
    assert(feature_data != NULL);
    assert(guess_word   != NULL);
    assert(new_word     != NULL);

    char feature_buf[MAX_INPUT_LEN] = "";
    int  feature_len = 0;

    printf("В отличие от %s %s ", guess_word, new_word);

    if (scanf("%1023[^\n]%n", feature_buf, &feature_len) != 1)
    {
        PRINTERR("Input error");
        return TREE_INVALID_INPUT;
    }

    CleanBuffer();

    char* feature = strdup(feature_buf);

    if (feature == NULL)
    {
        return TREE_CALLOC_ERROR;
    }

    *feature_data = feature;

    return TREE_SUCCESS;
}

//------------------------------------------------------------------------------------------

int FeatureHasNegatives(char* feature)
{
    assert(feature != NULL);

// TODO: внутри слова можно не

    if (strstr(feature, "не ") != NULL || strstr(feature, " не ") != NULL)
    {
        free(feature);
        return 1;
    }

    return 0;
}

//------------------------------------------------------------------------------------------
