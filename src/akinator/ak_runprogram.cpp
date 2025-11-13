#include "akinator.h"
#include "data_access.h"

//------------------------------------------------------------------------------------------

TreeErr_t AkinatorCtor(AkinatorCtx_t* ak_ctx)
{
    assert(ak_ctx != NULL);

    TreeErr_t error = TREE_SUCCESS;

    if ((error = TreeCtor(&ak_ctx->tree)))
    {
        return error;
    }

    ak_ctx->user_active = 0;
    ak_ctx->cmd         = AK_UNKNOWN_CMD;

    return TREE_SUCCESS;
}

//------------------------------------------------------------------------------------------

TreeErr_t AkinatorDtor(AkinatorCtx_t* ak_ctx)
{
    if (ak_ctx == NULL)
    {
        return TREE_NULL;
    }

    TreeErr_t error = TREE_SUCCESS;

    if ((error = TreeDtor(&ak_ctx->tree)))
    {
        return error;
    }

    ak_ctx->user_active = 0;
    ak_ctx->cmd         = AK_UNKNOWN_CMD;

    return TREE_SUCCESS;
}

//------------------------------------------------------------------------------------------

TreeErr_t AkinatorExecuteProgram(AkinatorCtx_t* ak_ctx)
{
    DEBUG_TREE_CHECK(&ak_ctx->tree, "ERROR BEFORE AKINATOR EXECUTE PROGRAM");

    TreeErr_t err = TREE_SUCCESS;

    ak_ctx->user_active = 1;
    ak_ctx->cmd = AK_UNKNOWN_CMD;

    while (ak_ctx->user_active)
    {
        ak_ctx->cmd = GetUserCommand(ak_ctx);

        if (ak_ctx->user_active)
        {
            if ((err = AkinatorExecuteCmd(ak_ctx)))
            {
                return err;
            }
        }

        if (ak_ctx->user_active)
        {
            int c = getchar();

            if (!(isspace(c)))
            {
                CleanBuffer();
            }
        }
    }

    DEBUG_TREE_CHECK(&ak_ctx->tree, "ERROR DUMP AFTER AKINATOR EXECUTE PROGRAM");

    return TREE_SUCCESS;
}

//------------------------------------------------------------------------------------------

AkinatorCmd_t GetUserCommand(AkinatorCtx_t* ak_ctx)
{
    char answer[MAX_INPUT_LEN] = {};

    printf( BLUE "Введите команду из списка " RESET_CLR "\n"
            "    [у] - угадывание слова,\n"       // guess
            "    [с] - сравнение слов,\n"         // compare
            "    [о] - определение слова,\n"      // describe
            "    [ч] - загрузить базу данных,\n"  // read
            "    [з] - записать в базу данных,\n" // write
            "    [в] - выход из программы \n"     // exit
            "\n" BLUE "Команда: " RESET_CLR);

    if (scanf("%63s", answer) != 1)
    {
        PRINTERR("Scanf failed");
        return AK_UNKNOWN_CMD;
    }

    CleanBuffer();

    printf("\n");

    if (strcmp(answer, "у") == 0)
    {
        return AK_GUESS;
    }
    if (strcmp(answer, "с") == 0)
    {
        return AK_COMPARE;
    }
    if (strcmp(answer, "о") == 0)
    {
        return AK_DESCRIBE;
    }
    if (strcmp(answer, "ч") == 0)
    {
        return AK_READ_DATA;
    }
    if (strcmp(answer, "з") == 0)
    {
        return AK_WRITE_DATA;
    }
    if (strcmp(answer, "в") == 0)
    {
        ak_ctx->user_active = 0;
        return AK_EXIT;
    }

    return AK_UNKNOWN_CMD;
}

//------------------------------------------------------------------------------------------

TreeErr_t AkinatorExecuteCmd(AkinatorCtx_t* ak_ctx)
{
    assert(ak_ctx != NULL);

    if (ak_ctx->cmd >= AK_CMD_COUNT || ak_ctx->cmd < 0)
    {
        PRINTERR("Unknown command %d", ak_ctx->cmd);
        return TREE_INVALID_INPUT;
    }

    TreeErr_t err = TREE_SUCCESS;

    if ((err = AK_EXECUTE_CMD_TABLE[ak_ctx->cmd](ak_ctx)))
    {
        return err;
    }

    return TREE_SUCCESS;
}

//------------------------------------------------------------------------------------------

TreeErr_t AkinatorExecuteReadData(AkinatorCtx_t* ak_ctx)
{
    assert(ak_ctx != NULL);

    TreeErr_t error = TREE_SUCCESS;

    if (ak_ctx->tree.size != 0)
    {
        if ((error = AkinatorDtor(ak_ctx)))
            return error;

        if ((error = AkinatorCtor(ak_ctx)))
            return error;

        ak_ctx->user_active = 1;
    }

    char file_path[MAX_INPUT_LEN] = {};

    printf(BLUE "Считывание дерева с базы данных:\n" RESET_CLR);

    printf(" Введите путь к базе данных: ");

    if (scanf("%1023[^\n]", file_path) != 1)
    {
        PRINTERR("Scanf failed");
        return TREE_INVALID_INPUT;
    }

    CleanBuffer();

    printf("\n");

    if ((error = TreeReadData(&ak_ctx->tree, file_path)))
    {
        return error;
    }

    printf(BLUE "База данных \"%s\" успешно считана!\n" RESET_CLR, file_path);

    return TREE_SUCCESS;
}

//------------------------------------------------------------------------------------------

TreeErr_t AkinatorExecuteWriteData(AkinatorCtx_t* ak_ctx)
{
    assert(ak_ctx != NULL);

    TreeErr_t error = TREE_SUCCESS;

    char file_path[MAX_INPUT_LEN] = {};

    printf(BLUE "Запись дерева в базу данных:\n" RESET_CLR);

    printf(" Введите путь к базе данных: ");

    if (scanf("%1023[^\n]", file_path) != 1)
    {
        PRINTERR("Scanf failed");
        return TREE_INVALID_INPUT;
    }

    CleanBuffer();

    printf("\n");

    if ((error = TreeWriteData(&ak_ctx->tree, file_path)))
    {
        return error;
    }

    printf(BLUE "База данных \"%s\" успешно записана!\n" RESET_CLR, file_path);

    return TREE_SUCCESS;
}

//------------------------------------------------------------------------------------------
