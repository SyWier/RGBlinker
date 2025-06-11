/*
 * log.c
 *
 *  Created on: Jun 11, 2025
 *      Author: Daniel
 */

#include "log.h"
#include <stdio.h>
#include <stdarg.h>

#define ESCAPE_NORM    "\033[0m"
#define ESCAPE_BLACK   "\033[30m"
#define ESCAPE_RED     "\033[31m"
#define ESCAPE_GREEN   "\033[32m"
#define ESCAPE_YELLOW  "\033[33m"
#define ESCAPE_BLUE    "\033[34m"
#define ESCAPE_MAGENTA "\033[35m"
#define ESCAPE_CYAN    "\033[36m"
#define ESCAPE_WHITE   "\033[37m"


#define LOG_ERR 1
#define LOG_WRR 2
#define LOG_INF 3
#define LOG_DBG 4

#define DEBUG_LEVEL LOG_DBG

void Log_Important(const char* format, ...) {
    printf(ESCAPE_MAGENTA "[!!!] ");

    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);

    printf(ESCAPE_NORM "\r\n");
}

void Log_Error(const char* format, ...) {
#if DEBUG_LEVEL >= LOG_ERR
    printf(ESCAPE_RED "[ERR] ");

    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);

    printf(ESCAPE_NORM "\r\n");
#endif
}

void Log_Warning(const char* format, ...) {
#if DEBUG_LEVEL >= LOG_WRR
    printf(ESCAPE_YELLOW "[WRR] ");

    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);

    printf(ESCAPE_NORM "\r\n");
#endif
}

void Log_Info(const char* format, ...) {
#if DEBUG_LEVEL >= LOG_INF
    printf(ESCAPE_GREEN "[INF] ");

    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);

    printf(ESCAPE_NORM "\r\n");
#endif
}

void Log_Debug(const char* format, ...) {
#if DEBUG_LEVEL >= LOG_DBG
    printf(ESCAPE_NORM "[DBG] ");

    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);

    printf(ESCAPE_NORM "\r\n");
#endif
}
