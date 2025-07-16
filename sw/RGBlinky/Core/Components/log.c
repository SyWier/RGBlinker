/*
 * log.c
 *
 *  Created on: Jun 11, 2025
 *      Author: Daniel
 */

// log.c
#include "main.h"
#include "log.h"
#include <stdarg.h>

// Global log level (can be changed at runtime)
#ifdef DEBUG
LogLevel global_log_level = LOG_DBG;
#else
LogLevel global_log_level = LOG_IMP;
#endif

static const char *level_colors[] = {
    "\x1b[1;35m", // Magenta - IMPORTANT
    "\x1b[1;31m", // Red     - ERROR
    "\x1b[1;33m", // Yellow  - WARNING
    "\x1b[1;36m", // Cyan    - INFO
    "\x1b[0;37m"  // Gray    - DEBUG
};

#define LOG_RESET_COLOR "\x1b[0m"

void log_message(LogLevel level, const char *tag, const char *fmt, ...) {
    if(level > global_log_level) return;  // Filter out lower-priority logs

    uint32_t ms = HAL_GetTick();  // Milliseconds since HAL init

    printf("%s[%lu ms][%s] ", level_colors[level], ms, tag);

    va_list args;
    va_start(args, fmt);
    vprintf(fmt, args);
    va_end(args);

    printf(LOG_RESET_COLOR "\r\n");
}
