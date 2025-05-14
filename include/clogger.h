// clogger.h
#ifndef CLOGGER_H
#define CLOGGER_H

#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

// Log levels
typedef enum {
    LOG_LEVEL_DEBUG,
    LOG_LEVEL_INFO,
    LOG_LEVEL_WARN,
    LOG_LEVEL_ERROR,
    LOG_LEVEL_FATAL
} LogLevel;

// Init logger
void clog_init(LogLevel level, const char *log_file_path);

void clog_close();

void clog_set_level(LogLevel level);

void clog_set_log_format(const char *format);

void clog_enable_colors(bool enable);

// Internal logging function
void clog_log(LogLevel level, const char *file, int line, const char *func, const char *fmt, ...);

// Logging macros
#define log_debug(...) clog_log(LOG_LEVEL_DEBUG, __FILE__, __LINE__, __func__, __VA_ARGS__)
#define log_info(...)  clog_log(LOG_LEVEL_INFO,  __FILE__, __LINE__, __func__, __VA_ARGS__)
#define log_warn(...)  clog_log(LOG_LEVEL_WARN,  __FILE__, __LINE__, __func__, __VA_ARGS__)
#define log_error(...) clog_log(LOG_LEVEL_ERROR, __FILE__, __LINE__, __func__, __VA_ARGS__)
#define log_fatal(...) clog_log(LOG_LEVEL_FATAL, __FILE__, __LINE__, __func__, __VA_ARGS__)

#ifdef __cplusplus
}
#endif

#endif // CLOGGER_H
