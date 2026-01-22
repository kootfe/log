//Every function comment is in log.c leave this file alone. This is just Header!
#ifndef LOG_H
#define LOG_H
#include <stdio.h>
#include <stdarg.h>

//Single logger operations:
typedef enum kl_logger_mode { LVL_DEBUG = 0,
    ON = 1,
    OFF = 2,
    FORCE_OFF = 3,
} kl_logger_mode_t;

typedef enum kl_log_level {
    ERROR,
    LOG,
    WARN,
    SUC,
    INFO,
    TRACE,
    FATAL,
    DEBUG,
} kl_log_level_t;

typedef struct kl_logger kl_logger_t;

int _kl_log(const kl_logger_t *loger, const kl_log_level_t level, const char *format, const char *fileinfo, const int line, const char *time, const char *date, ...);
int kl_can_log(kl_logger_t *lgr, const kl_log_level_t level);

#define protect(statement) do { statement } while (0);

//TODO: add kl_logger_t *def = kl_get_def_logger() for each non _sp macro for performance
#define klogf(format, ...) \
    protect( \
            if (kl_can_log(kl_get_def_logger(), LOG)) \
            _kl_log(kl_get_def_logger(), LOG, format, __FILE__, __LINE__, __TIME__, __DATE__, ##__VA_ARGS__); \
            )

#define klogf_sp(logger, format, ...) \
    protect( \
            if (kl_can_log(logger, LOG)) \
            _kl_log(logger, LOG, format, __FILE__, __LINE__, __TIME__, __DATE__, ##__VA_ARGS__); \
            )

#define errf(format, ...) \
    protect( \
            if (kl_can_log(kl_get_def_logger(), ERROR)) \
            _kl_log(kl_get_def_logger(), ERROR, format, __FILE__, __LINE__, __TIME__, __DATE__, ##__VA_ARGS__); \
            )

#define errf_sp(logger, format, ...) \
    protect( \
            if (kl_can_log(logger, ERROR)) \
            _kl_log(logger, ERROR, format, __FILE__, __LINE__, __TIME__, __DATE__, ##__VA_ARGS__); \
            )

#define warnf(format, ...) \
    protect( \
            if (kl_can_log(kl_get_def_logger(), WARN)) \
            _kl_log(kl_get_def_logger(), WARN, format, __FILE__, __LINE__, __TIME__, __DATE__, ##__VA_ARGS__); \
            )

#define warnf_sp(logger, format, ...) \
    protect( \
            if (kl_can_log(logger, WARN)) \
            _kl_log(logger, WARN, format, __FILE__, __LINE__, __TIME__, __DATE__, ##__VA_ARGS__); \
            )

#define sucf(format, ...) \
    protect( \
            if (kl_can_log(kl_get_def_logger(), SUC)) \
            _kl_log(kl_get_def_logger(), SUC, format, __FILE__, __LINE__, __TIME__, __DATE__, ##__VA_ARGS__); \
            )

#define sucf_sp(logger, format, ...) \
    protect( \
            if (kl_can_log(logger, SUC)) \
            _kl_log(logger, SUC, format, __FILE__, __LINE__, __TIME__, __DATE__, ##__VA_ARGS__); \
            )

#define infof(format, ...) \
    protect( \
            if (kl_can_log(kl_get_def_logger(), INFO)) \
            _kl_log(kl_get_def_logger(), INFO, format, __FILE__, __LINE__, __TIME__, __DATE__, ##__VA_ARGS__); \
            )

#define infof_sp(logger, format, ...) \
    protect( \
            if (kl_can_log(logger, INFO)) \
            _kl_log(logger, INFO, format, __FILE__, __LINE__, __TIME__, __DATE__, ##__VA_ARGS__); \
            )

#define fatalf(format, ...) \
    protect( \
            if (kl_can_log(kl_get_def_logger(), FATAL)) \
            _kl_log(kl_get_def_logger(), FATAL, format, __FILE__, __LINE__, __TIME__, __DATE__, ##__VA_ARGS__); \
            )

#define fatalf_sp(logger, format, ...) \
    protect( \
            if (kl_can_log(logger, FATAL)) \
            _kl_log(logger, FATAL, format, __FILE__, __LINE__, __TIME__, __DATE__, ##__VA_ARGS__); \
            )

#define traf(format, ...) \
    protect( \
            if (kl_can_log(kl_get_def_logger(), TRACE)) \
            _kl_log(kl_get_def_logger(), TRACE, format, __FILE__, __LINE__, __TIME__, __DATE__, ##__VA_ARGS__); \
            )

#define traf_sp(logger, format, ...) \
    protect( \
            if (kl_can_log(logger, TRACE)) \
            _kl_log(logger, TRACE, format, __FILE__, __LINE__, __TIME__, __DATE__, ##__VA_ARGS__); \
            )

#define debgf(format, ...) \
    protect( \
            if (kl_can_log(kl_get_def_logger(), DEBUG)) \
            _kl_log(kl_get_def_logger(), DEBUG, format, __FILE__, __LINE__, __TIME__, __DATE__, ##__VA_ARGS__); \
            )

#define debgf_sp(logger, format, ...) \
    protect( \
            if (kl_can_log(logger, DEBUG)) \
            _kl_log(logger, DEBUG, format, __FILE__, __LINE__, __TIME__, __DATE__, ##__VA_ARGS__); \
            )

int kl_set_mode(kl_logger_t *lgr, kl_logger_mode_t mode);
kl_logger_mode_t kl_get_mode(const kl_logger_t *logger);
int kl_set_file(kl_logger_t *lgr, FILE *file);
FILE *kl_get_file(const kl_logger_t *logger);

kl_logger_t kl_logger_init_stack(FILE *file, kl_logger_mode_t mode);
kl_logger_t *kl_logger_init_heap(FILE *file, kl_logger_mode_t mode);
int kl_logger_reset(kl_logger_t *lgr);
int kl_logger_destroy(kl_logger_t *lgr);

kl_logger_t *kl_logger_copy_stoh(kl_logger_t log);
kl_logger_t kl_logger_copy_htos(kl_logger_t *log);
kl_logger_t *kl_logger_copy_htoh(kl_logger_t *log);

int kl_set_def_logger(const kl_logger_t *lgr);
kl_logger_t *kl_get_def_logger(void);

//Multti logger perations:
//
//#define KL_MULTI_LOG // For dev side syntax higlighting. remove on commit
#ifdef KL_MULTI_LOG

//TODO: You need to make helper macros like errf and default array for fast usage.

typedef struct kl_log_array {
    kl_logger_t **loggers;
    size_t logger_count;
    size_t batch;
    size_t size;
} kl_log_array_t;

kl_log_array_t *_kl_create_log_array(size_t batch_size);
void kl_push_to_log_array_heap(kl_log_array_t *array, kl_logger_t *log);
kl_logger_t *kl_push_to_log_array_clone_stack(kl_log_array_t *array, kl_logger_t log);
void kl_clean_log_array(kl_log_array_t *array);
int _kl_log_arr(const kl_log_array_t *array, const kl_log_level_t level, const char *format, const char *fileinfo, const int line, const char *time, const char *date, ...);
#endif
#endif
//Why you are reading me?
