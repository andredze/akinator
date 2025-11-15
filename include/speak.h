#ifndef SPEAK_H
#define SPEAK_H

//——————————————————————————————————————————————————————————————————————————————————————————

#ifdef TREE_DEBUG
    #define DEBUG
#endif

//——————————————————————————————————————————————————————————————————————————————————————————

#include <stdio.h>
#include <stdarg.h>
#include "colors.h"
#include "common.h"

//——————————————————————————————————————————————————————————————————————————————————————————

int Speak(const char* const color, const char* fmt, ...);

int SpeakFlush();

int SpeakOnly(const char* fmt, ...);

int vSpeakOnly(const char* fmt, va_list args);

//——————————————————————————————————————————————————————————————————————————————————————————

const int SPEAK_BUFFER_SIZE = 1024;

//——————————————————————————————————————————————————————————————————————————————————————————

#endif /* SPEAK_H */
