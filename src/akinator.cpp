#include "akinator.h"

//------------------------------------------------------------------------------------------

TreeErr_t RunAkinator(Tree_t* tree)
{
    DEBUG_TREE_CHECK(tree, "ERROR BEFORE AKINATOR GUESS");

    if (tree->dummy->right == NULL)
    {
        MakeEmptyNode(tree);
    }

    int user_active = 1;

    while (user_active)
    {
        printf("Начать угадывание слова? ");

        int answer = GetUserAnswer();
        TreeErr_t err = TREE_SUCCESS;

        switch (answer)
        {
            case 'y':
                if ((err = AkinatorGuessWord(tree, &user_active)))
                {
                    return err;
                }
                break;

            case 'n':
                printf("Завершение программы, хорошего дня :)\n");
                user_active = 0;
                break;

            case EOF:
                return TREE_INVALID_INPUT;

            default:
                printf("Введите ответ в виде (да/нет)!\n");
                break;
        }
    }

    DEBUG_TREE_CHECK(tree, "ERROR DUMP AKINATOR GUESS");

    return TREE_SUCCESS;
}

//------------------------------------------------------------------------------------------

TreeErr_t MakeEmptyNode(Tree_t* tree)
{
    assert(tree != NULL);

    char* empty_data = (char*) calloc(sizeof(EMPTY_WORD) / sizeof(char), sizeof(char));

    if (empty_data == NULL)
    {
        return TREE_CALLOC_ERROR;
    }

    strcpy(empty_data, EMPTY_WORD);

    return TreeNodeCtor(tree, empty_data, &tree->dummy->right);
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
            printf("Я угадал хаха\n");
            *node_ptr = tree->dummy;
            break;

        case 'n':
            printf("Ебать ты кого там загадал(-а)\n");
            return AkinatorAddWord(tree, *node_ptr);

        default:
            return TREE_INVALID_INPUT;
    }

    return TREE_SUCCESS;
}

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
    if ((err = AkinatorGetFeature(&feature_data, guess_node->data, new_word_data)))
    {
        return err;
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

    char* new_word = (char*) calloc(new_word_len + 1, sizeof(char));

    if (new_word == NULL)
    {
        return TREE_CALLOC_ERROR;
    }

    strcpy(new_word, new_word_buf);

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

    char* feature = (char*) calloc(feature_len + 1, sizeof(char));

    if (feature == NULL)
    {
        return TREE_CALLOC_ERROR;
    }

    strcpy(feature, feature_buf);

    *feature_data = feature;

    return TREE_SUCCESS;
}

//------------------------------------------------------------------------------------------

int GetUserAnswer()
{
    char answer[MAX_INPUT_LEN] = {};
    int  answer_len = 0;

    // TODO: что если пользователь ввел слишком много

    if (scanf("%s%n", answer, &answer_len) != 1)
    {
        PRINTERR("Scanf failed");
        return EOF;
    }

    CleanBuffer();

    return GetShortAnswer(answer, answer_len + 1);
}

//------------------------------------------------------------------------------------------

int GetShortAnswer(char* str, int size)
{
    assert(str != NULL);

    char* lowered = (char*) calloc(size, sizeof(char));

    if (lowered == NULL)
    {
        PRINTERR("Memory allocation failed");
        return EOF;
    }

    StringToLower(lowered, str);

    if (strcmp(lowered, "да") == 0)
    {
        free(lowered);
        return 'y';
    }
    else if (strcmp(lowered, "нет") == 0)
    {
        free(lowered);
        return 'n';
    }
    else if (strcmp(lowered, "выход") == 0)
    {
        free(lowered);
        return 'e';
    }

    free(lowered);

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

char* StringToLower(char* dest, const char* src)
{
    assert(src  != NULL);
    assert(dest != NULL);

    char* start = dest;

    for (; *src != '\0'; dest++, src++)
    {
        *dest = ToLower(*src);
    }

    return start;
}

//------------------------------------------------------------------------------------------

char ToLower(char letter)
{
    /* English */
    if (letter >= 'A' && letter <= 'Z')
    {
        return letter + 'a' - 'A';
    }

    /* Russian */
    if (letter >= 'А' && letter <= 'Я')
    {
        return letter + 'а' - 'А';
    }

    return letter;
}

//------------------------------------------------------------------------------------------

TreeErr_t AkinatorWriteData(const Tree_t* tree)
{
    DEBUG_TREE_CHECK(tree, "ERROR BEFORE AKINATOR WRITE DATA");

    char data_file_path[MAX_FILE_NAME_LEN] = "";
    MakeDataFilePath(data_file_path);

    FILE* fp = fopen(data_file_path, "w");

    if (fp == NULL)
    {
        PRINTERR("Error with opening file: %s", data_file_path);
        return TREE_FILE_ERR;
    }

    TreeErr_t err = TREE_SUCCESS;

    if ((err = WriteNode(tree->dummy->right, fp)))
    {
        return err;
    }

    fclose(fp);

    printf("База данных записана в файл: %s\n", data_file_path);

    return TREE_SUCCESS;
}

//------------------------------------------------------------------------------------------

void MakeDataFilePath(char* data_file_path)
{
    time_t rawtime = time(NULL);

    struct tm* info = localtime(&rawtime);

    char time[MAX_TIME_LEN] = "";

    strftime(time, sizeof(time), "%d%m%Y_%H%M%S", info);

    snprintf(data_file_path, MAX_FILE_NAME_LEN, "data/%s.txt", time);
}

//------------------------------------------------------------------------------------------

TreeErr_t WriteNode(const TreeNode_t* node, FILE* fp)
{
    assert(node != NULL);

    TreeErr_t err = TREE_SUCCESS;

    fprintf(fp, "( \"%s\" ", node->data);

    if (node->left != NULL)
    {
        if ((err = WriteNode(node->left, fp)))
        {
            return err;
        }
    }
    if (node->right != NULL)
    {
        if ((err = WriteNode(node->right, fp)))
        {
            return err;
        }
    }

    fprintf(fp, ")");

    return TREE_SUCCESS;
}

//------------------------------------------------------------------------------------------

TreeErr_t AkinatorReadData(Tree_t* tree, const char* data_file_path)
{
    DEBUG_TREE_CHECK(tree, "ERROR BEFORE AKINATOR READ DATA");

    assert(data_file_path != NULL);

    FILE* fp = fopen(data_file_path, "r");

    if (fp == NULL)
    {
        PRINTERR("Error with opening file: %s", data_file_path);
        return TREE_FILE_ERR;
    }

    TreeErr_t err = TREE_SUCCESS;

    if ((err = ReadNode(tree->dummy, fp)))
    {
        return err;
    }

    DEBUG_TREE_CHECK(tree, "ERROR AFTER AKINATOR READ DATA");
    TREE_CALL_DUMP  (tree, "DUMP AFTER AKINATOR READ DATA");

    return TREE_SUCCESS;
}

//------------------------------------------------------------------------------------------

TreeErr_t ReadNode(TreeNode_t* node, FILE* fp)
{
    char buffer[MAX_INPUT_LEN] = {};
    int  data_len = 0;

    if (fscanf(fp, "(", buffer, &data_len) != 1)
    {
        PRINTERR("Error with reading data");
        return TREE_FILE_ERR;
    }

    if (fscanf(fp, "\"%[^\"]%n", buffer, &data_len) != 1)
    {
        PRINTERR("Error with reading data");
        return TREE_FILE_ERR;
    }


}

//------------------------------------------------------------------------------------------
