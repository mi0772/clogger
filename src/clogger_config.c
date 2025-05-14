//
// Created by Carlo Di Giuseppe on 14/05/25.
//

#include "clogger_config.h"
#include "clogger.h"
#include "toml.h"

#include <stdio.h>
#include <string.h>

bool clog_load_config(const char* config_path) {
    FILE* fp = fopen(config_path, "r");
    if (!fp) {
        fprintf(stderr, "[CLOGGER] Unable to open config file: %s\n", config_path);
        return false;
    }

    char errbuf[200];
    toml_table_t* conf = toml_parse_file(fp, errbuf, sizeof(errbuf));
    fclose(fp);

    if (!conf) {
        fprintf(stderr, "[CLOGGER] TOML parse error: %s\n", errbuf);
        return false;
    }

    // log_level
    const char* level_str = NULL;
    toml_datum_t d = toml_string_in(conf, "log_level");
    if (d.ok) level_str = d.u.s;

    LogLevel level = LOG_LEVEL_INFO;
    if (level_str) {
        if (strcmp(level_str, "DEBUG") == 0) level = LOG_LEVEL_DEBUG;
        else if (strcmp(level_str, "INFO") == 0) level = LOG_LEVEL_INFO;
        else if (strcmp(level_str, "WARN") == 0) level = LOG_LEVEL_WARN;
        else if (strcmp(level_str, "ERROR") == 0) level = LOG_LEVEL_ERROR;
        else if (strcmp(level_str, "FATAL") == 0) level = LOG_LEVEL_FATAL;
    }

    // use_colors
    bool use_colors = true;
    d = toml_bool_in(conf, "use_colors");
    if (d.ok) use_colors = d.u.b;

    // log_file
    const char* log_path = NULL;
    d = toml_string_in(conf, "log_file");
    if (d.ok) log_path = d.u.s;

    // output_stream
    const char* output_str = NULL;
    d = toml_string_in(conf, "output_stream");
    if (d.ok) output_str = d.u.s;

    // log_format
    toml_table_t* fmt = toml_table_in(conf, "format");
    const char* format = NULL;
    if (fmt) {
        d = toml_string_in(fmt, "log_format");
        if (d.ok) format = d.u.s;
    }

    clog_init(level, log_path);
    clog_enable_colors(use_colors);
    if (format) clog_set_log_format(format);
    //if (output_str && strcmp(output_str, "stdout") == 0) clog_set_output_stream(stdout);
    //else if (output_str && strcmp(output_str, "stderr") == 0) clog_set_output_stream(stderr);

    toml_free(conf);
    return true;
}