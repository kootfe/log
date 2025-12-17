#define _POSIX_C_SOURCE 200809L
#include "log.h"
#include <stdarg.h>
#include <stdio.h>
#include <unistd.h>

static const char *RESET = "\033[0m";

struct kl_logger {
    FILE *log_file;
    kl_logger_mode_t mode;
    int is_external_stream;
    int is_a_tty;
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
    [LOG] = { "LOG", "\033[34m", ON }
};

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
    if (mode < ON || mode > FORCE_OFF) return -1;
    lgr->mode = mode;
    return 0;
}

int is_external(FILE *f)
{
    return (f != stderr && stdout != f && f != stdin);
}

/*
 * Initializes a logger, NULL file defs to stderr..
 */
kl_logger_t kl_logger_init(FILE *file, kl_logger_mode_t mode)
{
    kl_logger_t lgr;
    FILE *fl = file ? file : stderr;
    lgr.log_file = fl;
    lgr.mode = mode;
    lgr.is_external_stream = is_external(fl);
    lgr.is_a_tty = isatty(fileno(fl));
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
    return 0;
}

kl_logger_t *kl_get_def_logger(void)
{
    return &default_logger;
}

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

int kl_log_impl(const kl_logger_t *lgr, const kl_log_level_t level, const char *format, const char *fileinfo, const int line, const char *time, const char *date, ...)
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

    if (mode < ON || mode > FORCE_OFF) return -1;
    struct kl_log_meta act = levels[level];

    if (mode > act.max_allowed_mode) return -1;
    kl_print_log(is_a_tty, logfile, format, act.color, act.label, fileinfo, line, time, date, args);

    va_end(args);
    return 0;
}

