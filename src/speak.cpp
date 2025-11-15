#define TX_USE_SPEAK
#include <TXLib.h>

#include "speak.h"

//------------------------------------------------------------------------------------------

static char   speak_buffer[SPEAK_BUFFER_SIZE] = {};
static size_t buffer_pos = 0;

//------------------------------------------------------------------------------------------

int Speak(const char* const color, const char* fmt, ...)
{
    DPRINTF("Entering speak\n");

    va_list args = {};

    va_start(args, fmt);

    if (color == NULL)
    {
        vprintf(fmt, args);
    }
    else
    {
        vcprintf(color, fmt, args);
    }

    int written = 0;

    char fmt_buffer[FMT_BUFFER_SIZE] = {};

    if ((written = vsnprintf(fmt_buffer, sizeof(fmt_buffer), fmt, args)) < 0)
    {
        PRINTERR("vsnprintf failed");
        return EOF;
    }

    if (buffer_pos + written >= SPEAK_BUFFER_SIZE)
    {
        SpeakFlush();
    }

    // DPRINTF("fmt_buffer = %s;\n", fmt_buffer);
    // DPRINTF("buffer_pos = %d;\n", buffer_pos);
    // DPRINTF("written    = %d;\n", written);

    memcpy(speak_buffer + buffer_pos, fmt_buffer, written);

    buffer_pos += written;

    va_end(args);

    // DPRINTF("speak_buf = %s\n", speak_buffer);

    return 0;
}

//------------------------------------------------------------------------------------------

int SpeakFlush()
{
    DPRINTF("FLUSH: speak_buf = %s\n", speak_buffer);

    if (buffer_pos == 0)
    {
        return 0;
    }

    speak_buffer[buffer_pos] = '\0';

    char tx_speak_buffer[SPEAK_BUFFER_SIZE * 2] = {};

    sprintf(tx_speak_buffer, "%s", speak_buffer); // <speak><prosody rate=\"fast\"> </prosody></speak>

    if (txSpeak(tx_speak_buffer) == EOF)
    {
        PRINTERR("txSpeak failed");
        return EOF;
    }

    buffer_pos = 0;

    return 0;
}

//------------------------------------------------------------------------------------------
