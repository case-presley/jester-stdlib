#include "jester/log/jester-log.h"

#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

static const char* log_level_names[] =
    {
        "\x1b[34m[DEBUG]\x1b[0m",
        "\x1b[32m[INFO]\x1b[0m",
        "\x1b[33m[WARN]\x1b[0m",
        "\x1b[31m[ERROR]\x1b[0m",
        "\x1b[35m[FATAL]\x1b[0m"
    };

static const char* log_level_plain[] = {"[DEBUG]", "[INFO]", "[WARN]", "[ERROR]", "[FATAL]"};

static LogQueue_t console_queue;
static LogQueue_t file_queue;

static LogConfig_t default_cfg = {
    .color_enabled = true,
    .file_enabled = true,
    .console_enabled = true,
    .min_log_level = DEBUG,
    .file_name = "log.txt",
    .sink = NULL,
    .sink_user_data = NULL,
    .file = NULL,
    .console_queue = &console_queue,
    .file_queue = &file_queue
};

static LogConfig_t log_cfg;

void set_cfg(const LogConfig_t* config)
{
    log_cfg = *config;
}

bool log_init(const LogConfig_t* cfg)
{
    log_cfg = cfg ? *cfg : default_cfg;

#ifdef _WIN32
    // Windows build
     log_cfg.color_enabled = false;
#else
    // Unix / macOS / WSL
    if (!isatty(fileno(stdout)) || getenv("TERM") == NULL)
        log_cfg.color_enabled = false;
    else
        log_cfg.color_enabled = true;
#endif

    const time_t now = time(NULL);
    const struct tm tm = *localtime(&now);
    strftime(log_cfg.file_name, sizeof(log_cfg.file_name), "logger_%m-%d-%Y.txt", &tm);

    if (log_cfg.file_enabled)
    {
        log_cfg.file = fopen(log_cfg.file_name, "a");
        if (!log_cfg.file)
        {
            fprintf(stderr, "Could not open log file %s\n", log_cfg.file_name);
            return false;
        }
    }
    return true;
}


void log_msg(const LogLevel_t level, const char *file, const int line, const char *format, ...)
{
    if (level < log_cfg.min_log_level) return;


    const time_t now = time(NULL);
    const struct tm tm = *localtime(&now);
    char t_buffer[32];
    strftime(t_buffer, sizeof(t_buffer), "%m-%d-%Y %H:%M:%S", &tm);

    va_list args;
    va_start(args, format);
    va_list args_copy;
    va_copy(args_copy, args);
    va_list args_copy2;
    va_copy(args_copy2, args);

    LogRecord_t record;

    record.level = level;

    strcpy(record.timestamp, t_buffer); // record.timestamp = the time buffer

    record.file = file;

    record.line = line;

    vsnprintf(record.message, sizeof(record.message), format, args);

    if (config.console_enabled)
    {
        enqueue(record, &config.console_queue);
    }
    if (config.file_enabled)
    {
        enqueue(record, &config.file_queue);
    }

    if (log_cfg.sink)
    {
        char message_buffer[512];
        vsnprintf(message_buffer, sizeof(message_buffer), format, args_copy2);
        log_cfg.sink(level, file, line, message_buffer, log_cfg.sink_user_data);
    }

    va_end(args);
    va_end(args_copy);
    va_end(args_copy2);
}

void enqueue(const LogRecord_t *record, const LogQueue_t *queue)
{
    if (queue->count == 128) log_flush();

    else
    {

    }
}

void log_flush()
{
    if (log_cfg.file)
    {
        fflush(log_cfg.file);
    }
}

void log_shutdown()
{
    if (log_cfg.file)
    {
        log_flush();
        fclose(log_cfg.file);
        log_cfg.file = NULL;
    }
}

void log_set_sink(LogSinkFn sink, void* user_data)
{
    log_cfg.sink = sink;
    log_cfg.sink_user_data = user_data;
}

void set_min_log_level(const LogLevel_t level)
{
    log_cfg.min_log_level = level;
}

void toggle_color(const bool enabled)
{
    log_cfg.color_enabled = enabled;
}

void toggle_file(const bool enabled)
{
    log_cfg.file_enabled = enabled;
}

