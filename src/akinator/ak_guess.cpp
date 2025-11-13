#include "akinator.h"

//------------------------------------------------------------------------------------------

TreeErr_t RunAkinator(Tree_t* tree)
{
    DEBUG_TREE_CHECK(tree, "ERROR BEFORE AKINATOR GUESS");

    TreeErr_t err = TREE_SUCCESS;

    if (tree->dummy->right == NULL)
    {
        /* Make empty word for easier program usage */
        if ((err = AkinatorNodeCtor(tree, EMPTY_WORD, &tree->dummy->right, tree->dummy)))
        {
            return err;
        }
    }

    int user_active = 1;
    int answer = 'y';

    while (user_active)
    {
        if ((err = AkinatorExecuteProgramOnce(tree, answer, &user_active)))
        {
            return err;
        }
        if (user_active)
        {
            printf("Начать угадывание слова заново? ");
            answer = GetUserAnswer();
        }
    }

    DEBUG_TREE_CHECK(tree, "ERROR DUMP AKINATOR GUESS");

    return TREE_SUCCESS;
}

//------------------------------------------------------------------------------------------

TreeErr_t AkinatorExecuteProgramOnce(Tree_t* tree, int answer, int* user_active)
{
    assert(user_active != NULL);

    TreeErr_t err = TREE_SUCCESS;

    switch (answer)
    {
        case 'y':
            if ((err = AkinatorGuessWord(tree, user_active)))
            {
                return err;
            }
            break;

        case 'n':
            printf("Завершение программы, хорошего дня :)\n");
            *user_active = 0;
            break;

        case EOF:
            return TREE_INVALID_INPUT;

        default:
            printf("Введите ответ в виде (да/нет)!\n");
            break;
    }

    return TREE_SUCCESS;
}

//------------------------------------------------------------------------------------------

TreeErr_t AkinatorNodeCtor(Tree_t* tree, const char* word, TreeNode_t** node_ptr, TreeNode_t* parent)
{
    assert(tree != NULL);

    char* data = strdup(word);

    if (data == NULL)
    {
        return TREE_CALLOC_ERROR;
    }

    (*node_ptr)->dynamic_memory = 1;

    return TreeNodeCtor(tree, data, node_ptr, parent);
}

//------------------------------------------------------------------------------------------

TreeErr_t AkinatorGuessWord(Tree_t* tree, int* user_active)
{
    assert(user_active != NULL);
    assert(tree        != NULL);

    TreeNode_t* node = tree->dummy->right;

    while (node != NULL)
    {
        printf("Твой персонаж %s? (да/нет): ", node->data);

        int answer = GetUserAnswer();

        if (answer == 'e')
        {
            *user_active = 0;
            break;
        }

        if (node->left == NULL && node->right == NULL)
        {
            return AkinatorMakeGuess(tree, &node, answer);
        }

        if (answer == 'y')
        {
            node = node->left;
        }
        else if (answer == 'n')
        {
            node = node->right;
        }
        else
        {
            printf("Введи ответ в виде (да/нет/выход)!\n");
            continue;
        }
    }

    return TREE_SUCCESS;
}

//------------------------------------------------------------------------------------------

TreeErr_t AkinatorMakeGuess(Tree_t* tree, TreeNode_t** node_ptr, int answer)
{
    assert(tree     != NULL);
    assert(node_ptr != NULL);

    switch (answer)
    {
        case 'y':
            printf("Я угадал бэумс\n");
            *node_ptr = tree->dummy;
            break;

        case 'n':
            printf("Капец ты кого там загадал(-а)\n");
            return AkinatorAddWord(tree, *node_ptr);

        default:
            return TREE_INVALID_INPUT;
    }

    return TREE_SUCCESS;
}

//------------------------------------------------------------------------------------------

int GetUserAnswer()
{
    char answer[MAX_INPUT_LEN] = {};

    if (scanf("%63s", answer) != 1)
    {
        PRINTERR("Scanf failed");
        return EOF;
    }

    CleanBuffer();

    return GetShortAnswer(answer);
}

//------------------------------------------------------------------------------------------

int GetShortAnswer(char* str)
{
    assert(str != NULL);

    if (strcmp(str, "да") == 0)
    {
        return 'y';
    }
    else if (strcmp(str, "нет") == 0)
    {
        return 'n';
    }
    else if (strcmp(str, "выход") == 0)
    {
        return 'e';
    }

    return '\0';
}

//------------------------------------------------------------------------------------------

void CleanBuffer()
{
    int c = '\0';

    while ((c = getchar()) != '\n' && c != EOF)
    {
        ; // do nothing
    }
}

//------------------------------------------------------------------------------------------
