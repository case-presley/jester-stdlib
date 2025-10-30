cmake_minimum_required(VERSION 4.0)
project(jester_stdlib C)

set(CMAKE_C_STANDARD 11)

add_library(jester_core STATIC
        src/log/jester_log.c
        include/jester/jester.h
        include/jester/data_structures/jester_data_structures.h
        tests/data_structures/jester_data_structures.c
        include/jester/data_structures/array/jester_array.h
        include/jester/data_structures/array/jester_dynamic_array.h
        src/data_structures/array/jester_dynamic_array.c)

target_include_directories(jester_core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_executable(jester_log tests/log/log_test.c)

# Link library + inherit include paths
target_link_libraries(jester_log PRIVATE jester_core)
