/*
 * log.h
 *
 *  Created on: Jun 11, 2025
 *      Author: Daniel
 */

#ifndef COMPONENTS_LOG_H_
#define COMPONENTS_LOG_H_

#include <stdio.h>

typedef enum {
    LOG_IMP = 0,
    LOG_ERR,
    LOG_WRN,
    LOG_INF,
    LOG_DBG,
} LogLevel;

extern LogLevel global_log_level;

void log_message(LogLevel level, const char *tag, const char *fmt, ...);

#endif /* COMPONENTS_LOG_H_ */
