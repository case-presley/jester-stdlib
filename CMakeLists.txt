cmake_minimum_required(VERSION 4.0)
project(jester_stdlib C)

set(CMAKE_C_STANDARD 11)

add_library(jester_core STATIC
        src/log/jester-log.c
        include/jester/jester.h
        include/jester/datastructs/jester-datastructs.h
        tests/datastructs/jester-datastructs.c
        include/jester/datastructs/array/jester-array.h
        include/jester/datastructs/array/jester-dynamic-array.h
        src/datastructs/array/jester-dynamic-array.c)

target_include_directories(jester_core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_executable(jester_log tests/log/log-test.c)

# Link library + inherit include paths
target_link_libraries(jester_log PRIVATE jester_core)
