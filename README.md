

# clogger

`clogger` is a lightweight, configurable, and thread-safe logging library for C99. It provides flexible log formatting, log levels, optional output to files, and configuration via TOML files.

## Features

- Log levels: DEBUG, INFO, WARN, ERROR, FATAL
- Customizable log format with placeholders
- Output to stderr, stdout, or file
- Colored output (ANSI)
- Thread-safe (uses `pthread_mutex`)
- Configuration via TOML

## Installation

Simply add the following files to your project:

```
include/clogger.h
src/clogger.c
src/clogger_config.c
external/toml.c
external/toml.h
```

Then include the `clogger` sources in your build system or compile them with your application.

## Usage

### Basic usage

```c
#include "clogger.h"

int main() {
    clog_init(LOG_LEVEL_DEBUG, "logfile.log");

    log_info("Application started");
    log_error("Something went wrong");

    clog_close();
    return 0;
}
```

### With configuration file

```c
#include "clogger_config.h"

int main() {
    if (!clog_load_config("clogger.toml")) {
        fprintf(stderr, "Failed to load configuration.\n");
        return 1;
    }

    log_debug("Debugging info");
    log_info("Startup complete");

    clog_close();
    return 0;
}
```

## Example TOML Configuration

```toml
log_level = "DEBUG"
use_colors = true
log_file = "logs/clogger.log"
output_stream = "stdout"

[format]
log_format = "[%LEVEL%] %TIME% [PID:%PID% TID:%THREAD%] - %MSG%"
```

## Format Placeholders

- `%LEVEL%`   — log level
- `%TIME%`    — timestamp
- `%FILE%`    — source file
- `%LINE%`    — line number
- `%FUNC%`    — function name
- `%MSG%`     — log message
- `%PID%`     — process ID
- `%THREAD%`  — thread ID

## License

MIT License