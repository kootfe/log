//Hello there. - Obi-Wan
//kootfe @ github, koofte @ discord.
//Every data is dd/mm/yyyy or dd/mm/yy in this file comments;
//entire KftLogger library source code:
#define _POSIX_C_SOURCE 200809L
#include "log.h"
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

struct kl_logger {
    FILE *log_file;
    kl_logger_mode_t mode;
    int is_external_stream;
    int is_a_tty;
    int is_heap;
};

struct kl_log_meta {
    const char *label;
    const char *color;
    int max_allowed_mode;
};

static kl_logger_t default_logger = {
    .log_file = NULL,
    .mode = ON,
    .is_external_stream = 0
};

//Needed for level comprasion. Sets the minimum needed levels per modes;
//TODO: Create min_needed_level() function for compare logic;
static const struct kl_log_meta levels[] = {
    [SUC] = { "SUCCESS", "\033[32m", ON },
    [WARN] = { "WARN", "\033[33m", ON },
    [ERROR] = { "ERROR", "\033[31m", OFF },
    [LOG] = { "LOG", "\033[34m", ON },
    [DEBUG] = { "DEBUG", "\033[36m", LVL_DEBUG },
    [TRACE] = { "TRACE", "\033[35m", LVL_DEBUG },
    [FATAL] = { "FATAL", "\033[31m", FORCE_OFF },
    [INFO] = { "INFO", "\033[34m", ON }
};

static const char *RESET = "\033[0m";

//Self explaining
//Just kidding. Decides if the file part of standarts or external stream;
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

/* 
 *Not used... Probally? Idk why i coded these but im too afraid to remove now.
 *Solved it, these are helper functions i use when i log the logger.
 *They are static so no need to remove.
 * TODO: Clean these functions and add intro logger's itself as meta logger;
 */
static inline void log_logger(kl_logger_t loger)
{
    printf("-----LOGGER-----\nLogger mode: %d\nIs A TTY: %d\nIs External: %d\nIs Heap: %d\n---------------\n", loger.mode, loger.is_a_tty, loger.is_external_stream, loger.is_heap);
}

static inline void log_requies(const char *text, int mode)
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
FILE *kl_get_file(const kl_logger_t *logger) {
    if (!logger) return NULL;
    return logger->log_file;
}

/*
 * POINTER to default logger.
 */
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

//Technichally can be done manualy, just safer helper;
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

//Dont forget to free it
kl_logger_t *kl_logger_init_heap(FILE *file, kl_logger_mode_t mode)
{
    kl_logger_t *lgr = malloc(sizeof(kl_logger_t));
    if (!lgr) return NULL;
    *lgr = kl_logger_init_stack(file, mode);
    lgr->is_heap = 1;
    return lgr;
}

//Heap to stack copy
kl_logger_t kl_logger_copy_htos(kl_logger_t *log)
{
    int fd = fileno(log->log_file);
    FILE *fl = fdopen(dup(fd), "w");
    kl_logger_t ret = {
        .is_heap = 0,
        .is_external_stream = log->is_external_stream,
        .log_file = fl,
        .is_a_tty = log->is_a_tty,
        .mode = log->mode,
    };
    return ret;
}

//stack to heap copy
kl_logger_t *kl_logger_copy_stoh(kl_logger_t log)
{
    int fd = fileno(log.log_file);
    FILE *fl = fdopen(dup(fd), "w");
    kl_logger_t *ret = kl_logger_init_heap(fl, log.mode);
    return ret;
}

//heap to heap copy
kl_logger_t *kl_logger_copy_htoh(kl_logger_t *log)
{
    int fd = fileno(log->log_file);
    FILE *fl = fdopen(dup(fd), "w");
    kl_logger_t *ret = kl_logger_init_heap(fl, log->mode);
    return ret;
}

//Does not free the logger. just resets to bare minimum!
//Still closes the file!
//file to stderr. 
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

//CLoses the file! And free's the logger!
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

#ifdef KL_MULTI_LOG //Im not sure if i should put these or not.
                    // Okay it gives error if not here lol.
                    // TODO: Learn why these defination guards are needed.

/* ---DEFINED IN log.h---
   typedef struct kl_log_array {
   kl_logger_t *loggers;
   size_t logger_count;
   size_t size;
   size_t batch;
   } kl_log_array_t;
*/

kl_log_array_t *_kl_create_log_array(size_t batch_size) {
    //If batch size is less then 1 we cant resize or add. (thats why we use size_t but again... 0)
    if (batch_size < 1) return NULL; //fun fact. on v1 this was return 0; which is same shit;
    kl_log_array_t *log_array = malloc(sizeof(kl_log_array_t));
    if (!log_array) return NULL;
    kl_logger_t **loggers = calloc(batch_size, sizeof(kl_logger_t*));
    if (!loggers) {
        free(log_array);
        return NULL;
    }
    log_array->loggers = loggers;
    log_array->logger_count = 0;
    log_array->batch = batch_size;
    log_array->size = batch_size;
    return log_array;
}


//DONE-TODO: by koofte @ github, 21/01/2026, Implement stack too!
void kl_push_to_log_array_heap(kl_log_array_t *array, kl_logger_t *log)
{
    if (array->logger_count + 1 > array->size) {
        size_t new_size = array->size + array->batch;
        kl_logger_t **tmp = realloc(array->loggers, sizeof(kl_logger_t*) * new_size); 
        if (!tmp) {
            errf("Failed to reallocate the logger array!");
            return;
        }
        array->loggers = tmp;
        array->size = new_size;
    }
    array->loggers[array->logger_count++] = log;
}

kl_logger_t *kl_push_to_log_array_clone_stack(kl_log_array_t *array, kl_logger_t log)
{
    kl_logger_t *logg = kl_logger_copy_stoh(log);
    kl_push_to_log_array_heap(array, logg);
    return logg;
}

//Just cleans the array. Not the loggers!
void kl_clean_log_array(kl_log_array_t *array)
{
    free(array->loggers);
    free(array);
}

//Destroy loggers too
void kl_destroy_log_array(kl_log_array_t *array)
{
    for (size_t i = 0; i < array->logger_count; ++i) {
        if (array->loggers[i]->is_heap) free(array->loggers[i]);
    }
    free(array->loggers);
    free(array);
}

int _kl_log_arr(const kl_log_array_t *array, const kl_log_level_t level, const char *format, const char *fileinfo, const int line, const char *time, const char *date, ...)
{
    va_list raw_args;
    va_start(raw_args, date);

    //TODO: Create a helper function taht globalizes logging logic for _kl_log and _kl_Log_arr
    for (size_t i = 0; i < array->logger_count; ++i) {
        kl_logger_t *lgr = array->loggers[i];
        va_list args;
        va_copy(args, raw_args);
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

    }
    va_end(raw_args);
    return 0;
}

#endif
//Bye bye!
