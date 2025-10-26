#ifndef JESTER_LOG_JESTER_LOG_H
#define JESTER_LOG_JESTER_LOG_H

#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>

typedef enum LogLevel
{
    DEBUG,
    INFO,
    WARNING,
    ERROR,
    FATAL
} LogLevel_t;

typedef void (*LogSinkFn)(LogLevel_t level, const char* file, int line, const char* message, void* user_data);

void log_set_sink(LogSinkFn sink, void* user_data);
void log_msg(LogLevel_t level, const char* file, int line, const char* format, ...);
void set_min_log_level(LogLevel_t level);
void toggle_color(bool enabled);
void toggle_file(bool enabled);
void log_flush(void);
void log_shutdown(void);

typedef struct LogRecord
{
    LogLevel_t level;
    const char* file;
    int line;
    char message[1024];
    char timestamp[32];
} LogRecord_t;


typedef struct LogQueue
{
    LogRecord_t records[SIZE];
    int head;
    int tail;
    int count;
} LogQueue_t;

typedef struct LogConfig
{
    bool color_enabled;
    bool file_enabled;
    bool console_enabled;
    LogLevel_t min_log_level;
    char file_name[128];
    LogSinkFn sink;
    void* sink_user_data;
    FILE* file;
    LogQueue_t console_queue;
    LogQueue_t file_queue;
} LogConfig_t;

bool log_init(const LogConfig_t* cfg);
void enqueue(const LogRecord_t* record, const LogQueue_t* queue);

#define LOG_DEBUG(format, ...)   log_msg(DEBUG,   __FILE__, __LINE__, format, ##__VA_ARGS__)
#define LOG_INFO(format, ...)    log_msg(INFO,    __FILE__, __LINE__, format, ##__VA_ARGS__)
#define LOG_WARNING(format, ...) log_msg(WARNING, __FILE__, __LINE__, format, ##__VA_ARGS__)
#define LOG_ERROR(format, ...)   log_msg(ERROR,   __FILE__, __LINE__, format, ##__VA_ARGS__)
#define LOG_FATAL(format, ...)   log_msg(FATAL,   __FILE__, __LINE__, format, ##__VA_ARGS__)

#define LOG_INIT_DEFAULT() log_init(NULL)
#define LOG_FLUSH()        log_flush()
#define LOG_SHUTDOWN()     log_shutdown()

#endif // JESTER_LOG_JESTER_LOG_H
