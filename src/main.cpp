#define   TX_USE_SPEAK
#define   TX_COMPILED
#include <TXLib.h>

#include "akinator.h"
#include "data_access.h"

//------------------------------------------------------------------------------------------

int main()
{
    AkinatorCtx_t ak_ctx = {};

    do {
        DPRINTF("Program start\n");

        if (AkinatorCtor(&ak_ctx))
            break;

        if (TreeReadData(&ak_ctx.tree, AK_DEFAULT_DATA_PATH))
            break;

        if (AkinatorExecuteProgram(&ak_ctx))
            break;

    } while (0);

    if (AkinatorDtor(&ak_ctx))
        return EXIT_FAILURE;

    return EXIT_SUCCESS;
}

//------------------------------------------------------------------------------------------
