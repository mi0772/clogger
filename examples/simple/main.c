//
// Created by Carlo Di Giuseppe on 14/05/25.
//
// main.c (example usage)
#include "../../include/clogger.h"

int main() {
    clog_init(LOG_LEVEL_DEBUG, "clogger.log");

    log_debug("Debug message: x = %d", 42);
    log_info("Information: everything is fine.");
    log_warn("Warning: disk space is low.");
    log_error("Error: cannot open file.");
    log_fatal("Fatal error: system crash imminent.");

    clog_close();
    return 0;
}