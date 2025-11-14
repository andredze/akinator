#include "speak.h"

//------------------------------------------------------------------------------------------

static char* speak_buffer[SPEAK_BUFFER_SIZE] = {};

//------------------------------------------------------------------------------------------

int Speak(const char* const color, const char* fmt, ...)
{
    va_list args = {};

    va_start(args, fmt);

    char fmt_buffer[MAX_OUTPUT_SIZE] = {};

    if (vsnprintf(fmt_buffer, sizeof(fmt_buffer), fmt, args) < 0)
    {
        PRINTERR("vsnprintf failed");
        return EOF;
    }

    if (txSpeak(fmt_buffer) == EOF)
    {
        PRINTERR("txSpeak failed");
        return EOF;
    }

    printf(fmt_buffer, "%s%s%s", color, print_buffer, RESET_COLOR);

    va_end(args);

    return 0;
}

//------------------------------------------------------------------------------------------
