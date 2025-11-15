#define   TX_USE_SPEAK
#define   TX_COMPILED
#include <TXLib.h>

#include "speak.h"

//------------------------------------------------------------------------------------------

static char   speak_buffer[SPEAK_BUFFER_SIZE] = {};
static size_t buffer_pos = 0;

//------------------------------------------------------------------------------------------

int Speak(const char* const color, const char* fmt, ...)
{
    // DPRINTF("Entering speak\n");

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

    int ret = vSpeakOnly(fmt, args);

    va_end(args);

    return ret;
}

//------------------------------------------------------------------------------------------

int SpeakOnly(const char* fmt, ...)
{
    va_list args = {};

    va_start(args, fmt);

    int ret = vSpeakOnly(fmt, args);

    va_end(args);

    return ret;
}

//------------------------------------------------------------------------------------------

int vSpeakOnly(const char* fmt, va_list args)
{
    // DPRINTF("Entering speak only\n");

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

    // DPRINTF("speak_buf = %s\n", speak_buffer);

    return 0;
}

//------------------------------------------------------------------------------------------

int SpeakFlush()
{
    // DPRINTF("FLUSH: speak_buf = %s\n", speak_buffer);

    if (buffer_pos == 0)
    {
        return 0;
    }

    speak_buffer[buffer_pos] = '\0';

    char tx_speak_buffer[SPEAK_BUFFER_SIZE * 2] = {};

    sprintf(tx_speak_buffer, "<speak version='1.0' xmlns='http://www.w3.org/2001/10/synthesis' xml:lang='RU'><prosody rate=\"fast\">"
                             "%s"
                             "</prosody></speak>", speak_buffer); // <speak><prosody rate=\"fast\"> </prosody></speak>

    if (txSpeak("%s", tx_speak_buffer) == EOF)
    {
        PRINTERR("txSpeak failed");
        return EOF;
    }

    buffer_pos = 0;

    return 0;
}

//------------------------------------------------------------------------------------------
