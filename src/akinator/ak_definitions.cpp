#include "akinator.h"

//------------------------------------------------------------------------------------------

TreeErr_t AkinatorCompareWords(Tree_t* tree, const char* word1, const char* word2)
{
    assert(word1 != NULL);
    assert(word2 != NULL);

    DEBUG_TREE_CHECK(tree, "ERROR BEFORE AKINATOR COMPARE WORDS");

    TreeErr_t error = TREE_SUCCESS;

    Step_t* word1_path = NULL;
    Step_t* word2_path = NULL;

    int pos1 = 0;
    int pos2 = 0;

    if ((error = FindWordPath(tree, word1, &word1_path, &pos1)))
    {
        return error;
    }
    if ((error = FindWordPath(tree, word2, &word2_path, &pos2)))
    {
        return error;
    }

    PrintComparison(word1_path, word2_path, pos1, pos2);

    free(word1_path);
    free(word2_path);

    return TREE_SUCCESS;
}

//------------------------------------------------------------------------------------------

void PrintComparison(Step_t* path1, Step_t* path2, int size1, int size2)
{
    assert(path1 != NULL);
    assert(path2 != NULL);

    if (size1 == 0 || size2 == 0)
    {
        return;
    }

    int pos1 = size1 - 1;
    int pos2 = size2 - 1;

    /* count end of common conditions */
    for (; pos1 > 0 && pos2 > 0 && StepsEqual(&path1[pos1], &path2[pos2]); pos1--, pos2--)
    {
        ; // do nothing
    }

    printf(BLUE "Сравнение %s и %s:\n" RESET_CLR, path1[0].word, path2[0].word);

    printf("%s похож на %s тем, что\n", path1[0].word, path2[0].word);

    printf("они оба ");

    PrintConditionsInReverse(path1, size1 - 1, pos1);

    printf("но %s ", path1[0].word);

    PrintConditionsInReverse(path1, pos1, 0);

    printf("a %s ", path2[0].word);

    PrintConditionsInReverse(path2, pos2, 0);

    printf("\n");
}

//------------------------------------------------------------------------------------------

void PrintConditionsInReverse(Step_t* words_path, int start_pos, int end_pos)
{
    int pos = start_pos;

    for (; pos > end_pos + 2; pos--)
    {
        PrintCondition(words_path[pos], ", ");
    }
    if (pos > end_pos + 1)
    {
        PrintCondition(words_path[pos], " и ");
        pos--;
    }
    if (pos > end_pos)
    {
        PrintCondition(words_path[pos], "\n");
        pos--;
    }
}

//------------------------------------------------------------------------------------------

int StepsEqual(Step_t* step1, Step_t* step2)
{
    if (step1->is_condition != step2->is_condition)
    {
        return 0;
    }
    if (strcmp(step1->word, step2->word) != 0)
    {
        return 0;
    }

    return 1;
}

//------------------------------------------------------------------------------------------

TreeErr_t AkinatorDescribeWord(Tree_t* tree, const char* word)
{
    assert(word != NULL);

    DEBUG_TREE_CHECK(tree, "ERROR BEFORE AKINATOR GET DEFINITION");

    TreeErr_t error = TREE_SUCCESS;
    Step_t* words_path = NULL;
    int pos = 0;

    if ((error = FindWordPath(tree, word, &words_path, &pos)))
    {
        return error;
    }

    PrintDescription(words_path, pos);

    free(words_path);

    return TREE_SUCCESS;
}

//------------------------------------------------------------------------------------------

TreeErr_t FindWordPath(Tree_t* tree, const char* word, Step_t** words_path, int* pos)
{
    assert(words_path != NULL);
    assert(word       != NULL);
    assert(tree       != NULL);
    assert(pos        != NULL);

    *words_path = (Step_t*) calloc(tree->size, sizeof(Step_t));

    if (*words_path == NULL)
    {
        PRINTERR("Memory allocation failed");
        return TREE_CALLOC_ERROR;
    }

    if (DescribeWord(tree->dummy->right, word, *words_path, pos) == 0)
    {
        printf("Word not found in database :(\n");
        free(*words_path);
        return TREE_NULL_NODE;
    }

    return TREE_SUCCESS;
}

//------------------------------------------------------------------------------------------

int DescribeWord(TreeNode_t* node, const char* word, Step_t* words_path, int* pos)
{
    if (node == NULL)
    {
        return 0;
    }

    if (strcmp(node->data, word) == 0)
    {
        words_path[*pos] = {.word = node->data, .is_condition = 1};
        (*pos)++;
        return 1;
    }
    if (DescribeWord(node->left, word, words_path, pos) == 1)
    {
        words_path[*pos] = {.word = node->data, .is_condition = 1};
        (*pos)++;
        return 1;
    }
    if (DescribeWord(node->right, word, words_path, pos) == 1)
    {
        words_path[*pos] = {.word = node->data, .is_condition = 0};
        (*pos)++;
        return 1;
    }


    return 0;
}

//------------------------------------------------------------------------------------------

void PrintDescription(Step_t* words_path, int size)
{
    assert(words_path != NULL);

    if (words_path[0].word == NULL)
    {
        return;
    }

    printf(BLUE "Определение %s:" RESET_CLR "\n %s ", words_path[0].word, words_path[0].word);

    for (int cur_pos = 1; cur_pos < size - 2; cur_pos++)
    {
        PrintCondition(words_path[cur_pos], ", ");
    }

    if (size > 2)
    {
        PrintCondition(words_path[size - 2], " и ");
    }
    if (size > 1)
    {
        PrintCondition(words_path[size - 1], ".\n");
    }

    printf("\n");
}

//------------------------------------------------------------------------------------------

void PrintCondition(Step_t step, const char* ending)
{
    if (step.is_condition == 1)
    {
        printf("%s%s", step.word, ending);
    }
    else
    {
        printf("не %s%s", step.word, ending);
    }
}

//------------------------------------------------------------------------------------------
