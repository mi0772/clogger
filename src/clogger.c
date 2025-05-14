//
// Created by Carlo Di Giuseppe on 14/05/25.
//

// clogger.c
#include "../include/clogger.h"
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>

static LogLevel current_level = LOG_LEVEL_INFO;
static FILE* log_file = NULL;
static bool use_colors = true;
static FILE* output_stream = NULL;
static char log_format[128] = "[%LEVEL%] %TIME% - %FILE%:%LINE%:%FUNC%(): %MSG%";
static pthread_mutex_t log_mutex = PTHREAD_MUTEX_INITIALIZER;

static const char* level_names[] = { "DEBUG", "INFO", "WARN", "ERROR", "FATAL" };
static const char* level_colors[] = { "\033[36m", "\033[32m", "\033[33m", "\033[31m", "\033[35m" }; // cyan, green, yellow, red, magenta

void clog_init(LogLevel level, const char* log_file_path) {
    if (!output_stream) {
        output_stream = stderr;
    }
    current_level = level;
    if (log_file_path) {
        log_file = fopen(log_file_path, "a");
        if (!log_file) {
            fprintf(stderr, "[CLOGGER] Failed to open log file: %s\n", log_file_path);
        }
    }
}

void clog_close() {
    if (log_file) {
        fclose(log_file);
        log_file = NULL;
    }
}

void clog_set_level(LogLevel level) {
    current_level = level;
}

void clog_set_log_format(const char* format) {
    if (format) {
        strncpy(log_format, format, sizeof(log_format) - 1);
        log_format[sizeof(log_format) - 1] = '\0';
    }
}

void clog_enable_colors(bool enable) {
    use_colors = enable;
}

void clog_log(LogLevel level, const char* file, int line, const char* func, const char* fmt, ...) {
    if (level < current_level) return;

    pthread_mutex_lock(&log_mutex);

    // Get time
    time_t now = time(NULL);
    struct tm* t = localtime(&now);
    char time_buf[20];
    strftime(time_buf, sizeof(time_buf), "%Y-%m-%d %H:%M:%S", t);

    pid_t pid = getpid();
    unsigned long tid = (unsigned long)pthread_self();

    // Format message
    char msg_buf[1024];
    va_list args;
    va_start(args, fmt);
    vsnprintf(msg_buf, sizeof(msg_buf), fmt, args);
    va_end(args);

    const char* color = use_colors ? level_colors[level] : "";
    const char* color_reset = use_colors ? "\033[0m" : "";

    // Costruzione log custom da formato
    char log_buf[2048];
    const char* p = log_format;
    char* out = log_buf;

    while (*p && (out - log_buf) < sizeof(log_buf) - 1) {
        if (strncmp(p, "%LEVEL%", 7) == 0) {
            out += snprintf(out, sizeof(log_buf) - (out - log_buf), "%s", level_names[level]);
            p += 7;
        } else if (strncmp(p, "%TIME%", 6) == 0) {
            out += snprintf(out, sizeof(log_buf) - (out - log_buf), "%s", time_buf);
            p += 6;
        } else if (strncmp(p, "%FILE%", 6) == 0) {
            out += snprintf(out, sizeof(log_buf) - (out - log_buf), "%s", file);
            p += 6;
        } else if (strncmp(p, "%LINE%", 6) == 0) {
            out += snprintf(out, sizeof(log_buf) - (out - log_buf), "%d", line);
            p += 6;
        } else if (strncmp(p, "%FUNC%", 6) == 0) {
            out += snprintf(out, sizeof(log_buf) - (out - log_buf), "%s", func);
            p += 6;
        } else if (strncmp(p, "%MSG%", 5) == 0) {
            out += snprintf(out, sizeof(log_buf) - (out - log_buf), "%s", msg_buf);
            p += 5;
        } else if (strncmp(p, "%PID%", 5) == 0) {
            out += snprintf(out, sizeof(log_buf) - (out - log_buf), "%d", pid);
            p += 5;
        } else if (strncmp(p, "%THREAD%", 8) == 0) {
            out += snprintf(out, sizeof(log_buf) - (out - log_buf), "%lu", tid);
            p += 8;
        } else {
            *out++ = *p++;
        }
    }
    *out = '\0';

    fprintf(output_stream, "%s%s%s\n", color, log_buf, color_reset);

    if (log_file) {
        fprintf(log_file, "%s\n", log_buf);
        fflush(log_file);
    }

    pthread_mutex_unlock(&log_mutex);
}

void clog_set_output_stream(FILE* stream) {
    output_stream = stream;
}
