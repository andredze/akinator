#include "akinator.h"

//------------------------------------------------------------------------------------------

TreeErr_t RunAkinator(Tree_t* tree)
{
    DEBUG_TREE_CHECK(tree, "ERROR BEFORE AKINATOR GUESS");

    TreeErr_t err = TREE_SUCCESS;

    if (tree->dummy->right == NULL)
    {
        /* Make empty word for easier program usage */
        if ((err = AkinatorNodeCtor(tree, EMPTY_WORD, &tree->dummy->right)))
        {
            return err;
        }
    }

    int user_active = 1;
    int answer = 'y';

    while (user_active)
    {
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

TreeErr_t AkinatorNodeCtor(Tree_t* tree, const char* word, TreeNode_t** node_ptr)
{
    assert(tree != NULL);

    char* data = strdup(word);

    if (data == NULL)
    {
        return TREE_CALLOC_ERROR;
    }

    return TreeNodeCtor(tree, data, node_ptr);
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
            printf("Капец ты кого там загадал(-а)\n");
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
    // if (letter >= 'А' && letter <= 'Я')
    // {
    //     return letter + 'а' - 'А';
    // }

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

    fprintf(fp, "(\"%s\"", node->data);

    if (node->left != NULL)
    {
        if ((err = WriteNode(node->left, fp)))
        {
            return err;
        }
    }
    else
    {
        fprintf(fp, "nil");
    }

    if (node->right != NULL)
    {
        if ((err = WriteNode(node->right, fp)))
        {
            return err;
        }
    }
    else
    {
        fprintf(fp, "nil");
    }

    fprintf(fp, ")");

    return TREE_SUCCESS;
}

//------------------------------------------------------------------------------------------

TreeErr_t AkinatorReadData(Tree_t* tree, const char* data_file_path)
{
    DEBUG_TREE_CHECK(tree, "ERROR BEFORE AKINATOR READ DATA");

    assert(data_file_path != NULL);

    DPRINTF("Reading file %s\n", data_file_path);

    FILE* fp = fopen(data_file_path, "r");

    if (fp == NULL)
    {
        PRINTERR("Error with opening file: %s", data_file_path);
        return TREE_FILE_ERR;
    }

    char* buffer = NULL;

    if (ReadFile(fp, &buffer, data_file_path))
    {
        return TREE_FILE_ERR;
    }

    TreeErr_t err = TREE_SUCCESS;
    int i = 0;

    if ((err = ReadNode(tree, &tree->dummy->right, buffer, &i)))
    {
        return err;
    }

    free(buffer);

    DEBUG_TREE_CHECK(tree, "ERROR AFTER AKINATOR READ DATA");
    TREE_CALL_DUMP  (tree, "DUMP AFTER AKINATOR READ DATA");

    return TREE_SUCCESS;
}

//------------------------------------------------------------------------------------------

TreeErr_t ReadNode(Tree_t* tree, TreeNode_t** node, char* buffer, int* i)
{
    DPRINTF("ReadNode(tree, %p);\n", node);

    assert(tree   != NULL);
    assert(node   != NULL);
    assert(buffer != NULL);

    TreeErr_t err = TREE_SUCCESS;

    if (SkipLetter(buffer, i, '('))
    {
        return TREE_FILE_ERR;
    }

    char* data = NULL;

    if ((err = ReadNodeData(buffer, i, &data)))
    {
        return err;
    }

    if ((err = TreeNodeCtor(tree, data, node)))
    {
        return err;
    }
    (*node)->data = data;

    if ((err = ReadNodeChild(tree, &(*node)->left, buffer, i)))
    {
        return err;
    }
    if ((err = ReadNodeChild(tree, &(*node)->right, buffer, i)))
    {
        return err;
    }

    if (SkipLetter(buffer, i, ')'))
    {
        return TREE_FILE_ERR;
    }

    return TREE_SUCCESS;
}

//------------------------------------------------------------------------------------------

TreeErr_t ReadNodeData(char* buffer, int* i, char** node_data)
{
    assert(node_data != NULL);
    assert(buffer    != NULL);
    assert(i         != NULL);

    char word[MAX_INPUT_LEN] = {};
    int  data_len = 0;

    if (sscanf(&buffer[*i], "\"%[^\"]\"%n", word, &data_len) != 1)
    {
        PRINTERR("Error with reading data");
        return TREE_FILE_ERR;
    }
    (*i) += data_len;

    DPRINTF("   word = %s;\n", word);

    char* data = strdup(word);

    if (data == NULL)
    {
        PRINTERR("Memory allocation failed");
        return TREE_CALLOC_ERROR;
    }

    *node_data = data;

    return TREE_SUCCESS;
}

//------------------------------------------------------------------------------------------

TreeErr_t ReadNodeChild(Tree_t* tree, TreeNode_t** child, char* buffer, int* i)
{
    assert(buffer != NULL);
    assert(child  != NULL);
    assert(i      != NULL);

    if (strncmp(&buffer[*i], "nil", 3) == 0)
    {
        (*i) += 3;
        return TREE_SUCCESS;
    }

    DPRINTF("   > ReadNode(tree, child=%p);\n", child);

    return ReadNode(tree, child, buffer, i);
}

//------------------------------------------------------------------------------------------

int SkipLetter(char* buffer, int* i, char letter)
{
    assert(buffer != NULL);
    assert(i      != NULL);

    if (buffer[*i] != letter)
    {
        PRINTERR("Error with reading %c, got %c (%d)", letter, buffer[*i], buffer[*i]);
        return 1;
    }
    (*i)++;

    DPRINTF("   Read %c\n", letter);

    return 0;
}

//------------------------------------------------------------------------------------------

int ReadFile(FILE* fp, char** buffer_ptr, const char* file_path)
{
    assert(fp         != NULL);
    assert(buffer_ptr != NULL);

    size_t size = 0;

    if (CountSize(file_path, &size))
    {
        return 1;
    }

    char* buffer = (char*) calloc(size / sizeof(char), sizeof(char));

    if (buffer == NULL)
    {
        PRINTERR("Memory allocation failed");
        return 1;
    }

    if (fread(buffer, size - 1, 1, fp) != 1)
    {
        PRINTERR("Reading file error");
        return 1;
    }

    buffer[size - 1] = '\0';

    DPRINTF("buffer last 2 chars: %d == %c; %d == %c\n",
            buffer[size - 2], buffer[size - 2],
            buffer[size - 1], buffer[size - 1]);

    *buffer_ptr = buffer;

    return 0;
}

//------------------------------------------------------------------------------------------

int CountSize(const char* file_path, size_t* size)
{
    struct stat fileinfo = {};

    if (stat(file_path, &fileinfo) == -1)
    {
        PRINTERR("Error with stat()");
        return 1;
    }

    *size = fileinfo.st_size + 1;

    return 0;
}

//------------------------------------------------------------------------------------------

TreeErr_t AkinatorDescribeWord(Tree_t* tree, const char* word)
{
    assert(word != NULL);

    DEBUG_TREE_CHECK(tree, "ERROR BEFORE AKINATOR GET DEFINITION");

    if (DescribeWord(tree->dummy->right, word) == -1)
    {
        return TREE_NULL_NODE;
    }
    printf("\n");

    return TREE_SUCCESS;
}

//------------------------------------------------------------------------------------------

int DescribeWord(TreeNode_t* node, const char* word)
{
    if (node == NULL)
    {
        return 0;
    }

    if (strcmp(node->data, word) == 0)
    {
        printf("%s ", node->data);
        return 1;
    }
    if (DescribeWord(node->left, word) == 1)
    {
        printf("%s ", node->data);
        return 1;
    }
    if (DescribeWord(node->right, word) == 1)
    {
        printf("не %s ", node->data);
        return 1;
    }

    return 0;
}

//------------------------------------------------------------------------------------------
//
// TreeErr_t AkinatorGetDefinition(Tree_t* tree, const char* word)
// {
//     assert(word != NULL);
//
//     DEBUG_TREE_CHECK(tree, "ERROR BEFORE AKINATOR GET DEFINITION");
//
//     TreeErr_t err = TREE_SUCCESS;
//     Stack_t stack = {};
//
//     if (StackCtor(&stack, 64))
//     {
//         return TREE_STACK_ERR;
//     }
//
//     int end_recursion = 0;
//
//     if (GetNodeDefinition(tree->dummy->right, &stack, word, &end_recursion))
//     {
//         StackDtor(&stack);
//         return err;
//     }
//
//     if (StackDtor(&stack))
//     {
//         return TREE_STACK_ERR;
//     }
//
//     return TREE_SUCCESS;
// }
//
// //------------------------------------------------------------------------------------------
//
// TreeErr_t GetNodeDefinition(TreeNode_t* node, Stack_t* stack, const char* word, int* end_recursion)
// {
//     assert(node != NULL);
//
//     DPRINTF("Handling word: %s\n", node->data);
//
//     if (strcmp(node->data, word) == 0)
//     {
//         DPRINTF("Нашел!! =))) %s\n", node->data);
//         PrintWordDefinition(word, stack);
//         *end_recursion = 1;
//         return TREE_SUCCESS;
//     }
//
//     TreeErr_t err = TREE_SUCCESS;
//
//     DPRINTF("pushing: %s..\n", node->data);
//
//     if (StackPush(stack, node))
//     {
//         return TREE_STACK_ERR;
//     }
//
//     if (node->left != NULL)
//     {
//         if ((err = GetNodeDefinition(node->left, stack, word, end_recursion)))
//         {
//             return err;
//         }
//         if (*end_recursion)
//         {
//             return TREE_SUCCESS;
//         }
//     }
//     if (node->right != NULL)
//     {
//         if ((err = GetNodeDefinition(node->right, stack, word, end_recursion)))
//         {
//             return err;
//         }
//         if (*end_recursion)
//         {
//             return TREE_SUCCESS;
//         }
//     }
//
//     TreeNode_t* last_node;
//     if (StackPop(stack, &last_node))
//     {
//         return TREE_STACK_ERR;
//     }
//     DPRINTF("poping: %s..\n", node->data);
//
//     return TREE_SUCCESS;
// }
//
// //------------------------------------------------------------------------------------------
//
// TreeErr_t PrintWordDefinition(const char* word, Stack_t* stack)
// {
//     assert(word  != NULL);
//     assert(stack != NULL);
//
//     DPRINTF("stack->size = %zu\n", stack->size);
//
//     char** features = (char**) calloc(stack->size, sizeof(char*));
//
//     if (features == NULL)
//     {
//         return TREE_CALLOC_ERROR;
//     }
//
//     TreeNode_t* node = NULL;
//
//     for (int i = stack->size - 1; i >= 0; i--)
//     {
//         DPRINTF("i = %d;\n", i);
//
//         if (StackPop(stack, &node))
//         {
//             free(features);
//             return TREE_STACK_ERR;
//         }
//
//         DPRINTF("poped: %s\n", node->data);
//
//         features[i] = node->data;
//
//         DPRINTF("features[%d] = %s;\n", i, features[i]);
//     }
//
//     for (int i = 0; i < stack->size; i++)
//     {
//         DPRINTF("features[%d] = %s;\n", i, features[i]);
//         printf("features[%d] = %s;\n", i, features[i]);
//         fflush(stdout);
//     }
//
//     free(features);
//
//     return TREE_SUCCESS;
// }
//
//------------------------------------------------------------------------------------------
