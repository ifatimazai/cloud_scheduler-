#include "../include/logger.h"
#include <stdlib.h>
#include <string.h>

static FILE* log_file = NULL;

void logger_init(const char* filename) {
    log_file = fopen(filename, "w");
    if (log_file) {
        fprintf(log_file, "=== Cloud Resource Scheduler Log ===\n");
        fprintf(log_file, "Started at: %s\n\n", ctime(&(time_t){time(NULL)}));
    }
}

void logger_log(LogLevel level, const char* message) {
    time_t now = time(NULL);
    struct tm* timeinfo = localtime(&now);
    char timestamp[20];
    strftime(timestamp, sizeof(timestamp), "%H:%M:%S", timeinfo);
    
    const char* level_str = logger_get_level_string(level);
    
    // Print to console
    printf("[%s] %s: %s\n", timestamp, level_str, message);
    
    // Log to file
    if (log_file) {
        fprintf(log_file, "[%s] %s: %s\n", timestamp, level_str, message);
        fflush(log_file);
    }
}

void logger_close() {
    if (log_file) {
        fprintf(log_file, "\n=== Log Ended ===\n");
        fclose(log_file);
        log_file = NULL;
    }
}

void logger_print_timestamp() {
    time_t now = time(NULL);
    printf("[%s] ", ctime(&now));
}

char* logger_get_level_string(LogLevel level) {
    switch (level) {
        case LOG_INFO: return "INFO";
        case LOG_WARNING: return "WARNING";
        case LOG_ERROR: return "ERROR";
        case LOG_SUCCESS: return "SUCCESS";
        default: return "UNKNOWN";
    }
}
