cmake_minimum_required(VERSION 3.15)
project(clogger C)

set(CMAKE_C_STANDARD 99)

# Includi headers
include_directories(include)

# Libreria statica clogger
add_library(clogger STATIC
        src/clogger.c
        src/clogger_config.c
        src/toml.c
)

# Eseguibile di esempio
add_executable(simple examples/simple/main.c)
add_executable(config examples/config/main.c)

# Linka clogger e pthread all'eseguibile
target_link_libraries(simple clogger pthread)
target_link_libraries(config clogger pthread)