//
// Created by Carlo Di Giuseppe on 14/05/25.
//

#include "../../include/clogger_config.h"
#include "../../include/clogger.h"

int main() {
    // Specifica il percorso del file TOML
    const char* config_path = "clogger.toml";

    // Carica la configurazione
    if (!clog_load_config(config_path)) {
        fprintf(stderr, "Errore durante il caricamento della configurazione da %s\n", config_path);
        return 1;
    }

    // Log di esempio
    log_debug("Debug message: x = %d", 42);
    log_info("Information: everything is fine.");
    log_warn("Warning: disk space is low.");
    log_error("Error: cannot open file.");
    log_fatal("Fatal error: system crash imminent.");

    clog_close();
    return 0;
}