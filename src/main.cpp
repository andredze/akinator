#define TX_USE_SPEAK
#include <TXLib.h>

#include "akinator.h"
#include "data_access.h"

//------------------------------------------------------------------------------------------

int main()
{

//     txSpeak ("TX Library is cool!");
//
//     txSpeak ("\vДобрый вечер, котики. Вы все мыши. Goog evening, kittens. You are all mices.");
//
//     txSpeak ("<speak version='1.0' xmlns='http://www.w3.org/2001/10/synthesis' xml:lang='EN'>"  // SSML format
//             "Goog evening, kittens. You are all mices."
//             "</speak>");

    Speak(NULL, "писюн");
    SpeakFlush();

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
