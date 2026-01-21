#define _POSIX_C_SOURCE 200809L
#include "log.h"
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

static const char *RESET = "\033[0m";

struct kl_logger {
    FILE *log_file;
    kl_logger_mode_t mode;
    int is_external_stream;
    int is_a_tty;
    int is_heap;
};

static kl_logger_t default_logger = {
    .log_file = NULL,
    .mode = ON,
    .is_external_stream = 0
};

struct kl_log_meta {
    const char *label;
    const char *color;
    int max_allowed_mode;
};

static struct kl_log_meta levels[] = {
    [SUC] = { "SUCCESS", "\033[32m", ON },
    [WARN] = { "WARN", "\033[33m", ON },
    [ERROR] = { "ERROR", "\033[31m", OFF },
    [LOG] = { "LOG", "\033[34m", ON },
    [DEBUG] = { "DEBUG", "\033[36m", LVL_DEBUG },
    [TRACE] = { "TRACE", "\033[35m", LVL_DEBUG },
    [FATAL] = { "FATAL", "\033[31m", FORCE_OFF },
    [INFO] = { "INFO", "\033[34m", ON }
};


static inline int is_external(FILE *f) { return (f != stderr && f != stdout && f != stdin); }
int kl_print_log(int is_a_tty, FILE *logfile, const char *format, const char *color, const char *level, const char *file, const int line,
        const char *time, const char *date, va_list args)
{
    if (is_a_tty) fprintf(logfile, "%s", color);
    fprintf(logfile, "[%s] { %s:%d, - %s %s } ", level, file, line, time, date);
    vfprintf(logfile, format, args);
    if (is_a_tty) fprintf(logfile, "%s", RESET);
    //   fflush(logfile);
    return 0;
}

void log_logger(kl_logger_t loger)
{
    printf("-----LOGGER-----\nLogger mode: %d\nIs A TTY: %d\nIs External: %d\nIs Heap: %d\n---------------\n", loger.mode, loger.is_a_tty, loger.is_external_stream, loger.is_heap);
}

void log_requies(const char *text, int mode)
{
    printf("-----REQUEST-----\nMessage: %s\nMode: %d\n---------------\n", text, mode);
}
/*
 * If NULL logger provided, retuns FORCE_OFF;
 */
kl_logger_mode_t kl_get_mode(const kl_logger_t *logger)
{
    if (!logger) return FORCE_OFF;
    return logger->mode;
}

/*
 * If NULL logger provided, return NULL;
 */
FILE *kl_get_file(const kl_logger_t *logger)
{
    if (!logger) return NULL;
    return logger->log_file;
}

kl_logger_t *kl_get_def_logger(void)
{
    return &default_logger;
}

/*
 * Replaces the logger's output stream.
 * If the previous stream was external (not stdin, stderr, stdout), it is closed.
 * NULL file falls back to stderr.
 */
int kl_set_file(kl_logger_t *lgr, FILE *file)
{
    if (!lgr) return -1;
    if (lgr->is_external_stream && lgr->log_file)
        fclose(lgr->log_file);

    lgr->log_file = file ? file : stderr;
    lgr->is_external_stream = (file != stderr && file != stdout && file != stdin);
    return 0;
}

int kl_set_mode(kl_logger_t *lgr, kl_logger_mode_t mode)
{
    if (!lgr) return -1;
    if (mode < LVL_DEBUG  || mode > FORCE_OFF) return -1;
    lgr->mode = mode;
    return 0;
}

/*
 * sets the default logger to given logger.
 * Any previously open external log file is close.
 * NULL logger files falls back to stderr.
 */
int kl_set_def_logger(const kl_logger_t *lgr)
{
    if (!lgr) return -1;
    if (default_logger.is_external_stream && default_logger.log_file) fclose(default_logger.log_file);
    FILE *f = lgr->log_file ? lgr->log_file : stderr;
    default_logger.log_file = f;
    default_logger.is_a_tty = isatty(fileno(f));
    default_logger.is_external_stream = is_external(f);
    default_logger.mode = lgr->mode;
    return 0;
}

/*
 * Initializes a logger, NULL file defs to stderr..
 */
kl_logger_t kl_logger_init_stack(FILE *file, kl_logger_mode_t mode)
{
    kl_logger_t lgr;
    FILE *fl = file ? file : stderr;
    lgr.log_file = fl;
    lgr.mode = mode;
    lgr.is_external_stream = is_external(fl);
    lgr.is_a_tty = isatty(fileno(fl));
    lgr.is_heap = 0;
    return lgr;
}

kl_logger_t *kl_logger_init_heap(FILE *file, kl_logger_mode_t mode)
{
    kl_logger_t *lgr = malloc(sizeof(kl_logger_t));
    if (!lgr) return NULL;
    *lgr = kl_logger_init_stack(file, mode);
    lgr->is_heap = 1;
    return lgr;
}

int kl_logger_reset(kl_logger_t *lgr)
{
    if (!lgr) return -1;
    if (lgr->is_external_stream && lgr->log_file) {
        fclose(lgr->log_file);
    }
    lgr->log_file = stderr;
    lgr->is_a_tty = isatty(fileno(stderr));
    lgr->is_external_stream = 0;
    return 0;
}

int kl_logger_destroy(kl_logger_t *lgr)
{
    if (!lgr) return -1;
    if (!lgr->is_heap) return -1;
    if (lgr->is_external_stream && lgr->log_file)
        fclose(lgr->log_file);
    lgr->is_external_stream = 0;
    lgr->is_a_tty = 0;
    free(lgr);
    return 0;
}



int _kl_log(const kl_logger_t *lgr, const kl_log_level_t level, const char *format, const char *fileinfo, const int line, const char *time, const char *date, ...)
{
    va_list args;
    va_start(args, date);

    FILE *logfile = stderr;
    kl_logger_mode_t mode = ON;
    int is_a_tty;
    if (lgr) {
        logfile = lgr->log_file ? lgr->log_file : stderr;
        mode = lgr->mode;
        is_a_tty = lgr->is_a_tty;
    } else is_a_tty = isatty(fileno(logfile));
    if (mode < LVL_DEBUG || mode > FORCE_OFF) return -1;
    struct kl_log_meta act = levels[level];
    if ((int) mode > (int) act.max_allowed_mode) return -1;
    kl_print_log(is_a_tty, logfile, format, act.color, act.label, fileinfo, line, time, date, args);

    va_end(args);
    return 0;
}
