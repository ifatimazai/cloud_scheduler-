#ifndef LOGGER_H
#define LOGGER_H

#include <stdio.h>
#include <time.h>

typedef enum {
    LOG_INFO,
    LOG_WARNING,
    LOG_ERROR,
    LOG_SUCCESS
} LogLevel;

void logger_init(const char* filename);
void logger_log(LogLevel level, const char* message);
void logger_close();
void logger_print_timestamp();
char* logger_get_level_string(LogLevel level);

#endif
