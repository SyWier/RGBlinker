/*
 * log.h
 *
 *  Created on: Jun 11, 2025
 *      Author: Daniel
 */

#ifndef COMPONENTS_LOG_H_
#define COMPONENTS_LOG_H_

void Log_Important(const char* format, ...);
void Log_Error(const char* format, ...);
void Log_Warning(const char* format, ...);
void Log_Info(const char* format, ...);
void Log_Debug(const char* format, ...);

#endif /* COMPONENTS_LOG_H_ */
