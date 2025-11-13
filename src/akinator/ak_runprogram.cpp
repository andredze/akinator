#include "akinator.h"

//------------------------------------------------------------------------------------------

TreeErr_t AkinatorExecuteProgram(AkinatorCtx_t* ak_ctx)
{
    DEBUG_TREE_CHECK(ak_ctx->tree, "ERROR BEFORE AKINATOR EXECUTE PROGRAM");

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
    }

    DEBUG_TREE_CHECK(ak_ctx->tree, "ERROR DUMP AFTER AKINATOR EXECUTE PROGRAM");

    return TREE_SUCCESS;
}

//------------------------------------------------------------------------------------------

AkinatorCmd_t GetUserCommand(AkinatorCtx_t* ak_ctx)
{
    char answer[MAX_INPUT_LEN] = {};

    printf("Введите команду из списка \n{"
            "\t[у] - угадывание слова,\n"       // guess
            "\t[с] - сравнение слов,\n"         // compare
            "\t[о] - определение слова,\n"      // describe
            "\t[ч] - загрузить базу данных,\n"  // read
            "\t[з] - записать в базу данных,\n" // write
            "\t[в] - выход из программы }\n");  // exit

    if (scanf("%63s", answer) != 1)
    {
        PRINTERR("Scanf failed");
        return EOF;
    }

    CleanBuffer();

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
    if (strcmp(answer, "я") == 0)
    {
        return AK_READ;
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
