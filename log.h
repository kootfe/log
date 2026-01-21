#ifndef LOG_H
#define LOG_H
#include <stdio.h>
#include <stdarg.h>

typedef enum kl_logger_mode_t {
    LVL_DEBUG = 0,
    ON = 1,
    OFF = 2,
    FORCE_OFF = 3,
} kl_logger_mode_t;

typedef enum kl_log_level_t {
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

#define klogf(format, ...) _kl_log(kl_get_def_logger(), LOG, format, __FILE__, __LINE__, __TIME__, __DATE__, ##__VA_ARGS__)
#define klogf_sp(loger, format, ...) _kl_log(loger, LOG, format, __FILE__, __LINE__, __TIME__, __DATE__, ##__VA_ARGS__)
#define errf(format, ...) _kl_log(kl_get_def_logger(), ERROR, format, __FILE__, __LINE__, __TIME__, __DATE__, ##__VA_ARGS__)
#define errf_sp(loger, format, ...) _kl_log(loger, ERROR, format, __FILE__, __LINE__, __TIME__, __DATE__, ##__VA_ARGS__)
#define warnf(format, ...) _kl_log(kl_get_def_logger(), WARN, format, __FILE__, __LINE__, __TIME__, __DATE__, ##__VA_ARGS__)
#define warnf_sp(loger, format, ...) _kl_log(loger, WARN, format, __FILE__, __LINE__, __TIME__, __DATE__, ##__VA_ARGS__)
#define sucf(format, ...) _kl_log(kl_get_def_logger(), SUC, format, __FILE__, __LINE__, __TIME__, __DATE__, ##__VA_ARGS__)
#define sucf_sp(loger, format, ...) _kl_log(loger, SUC, format, __FILE__, __LINE__, __TIME__, __DATE__, ##__VA_ARGS__)
#define infof(format, ...) _kl_log(kl_get_def_logger(), INFO, format, __FILE__, __LINE__, __TIME__, __DATE__, ##__VA_ARGS__)
#define infof_sp(loger, format, ...) _kl_log(loger, INFO, format, __FILE__, __LINE__, __TIME__, __DATE__, ##__VA_ARGS__)
#define fatalf(format, ...) _kl_log(kl_get_def_logger(), FATAL, format, __FILE__, __LINE__, __TIME__, __DATE__, ##__VA_ARGS__)
#define fatalf_sp(loger, format, ...) _kl_log(loger, FATAL, format, __FILE__, __LINE__, __TIME__, __DATE__, ##__VA_ARGS__)
#define traf(format, ...) _kl_log(kl_get_def_logger(), TRACE, format, __FILE__, __LINE__, __TIME__, __DATE__, ##__VA_ARGS__)
#define traf_sp(loger, format, ...) _kl_log(loger, TRACE, format, __FILE__, __LINE__, __TIME__, __DATE__, ##__VA_ARGS__)
#define debgf(format, ...) _kl_log(kl_get_def_logger(), DEBUG, format, __FILE__, __LINE__, __TIME__, __DATE__, ##__VA_ARGS__)
#define debgf_sp(loger, format, ...) _kl_log(loger, DEBUG, format, __FILE__, __LINE__, __TIME__, __DATE__, ##__VA_ARGS__)

int kl_set_mode(kl_logger_t *lgr, kl_logger_mode_t mode);
kl_logger_mode_t kl_get_mode(const kl_logger_t *logger);
int kl_set_file(kl_logger_t *lgr, FILE *file);
FILE *kl_get_file(const kl_logger_t *logger);

kl_logger_t kl_logger_init_stack(FILE *file, kl_logger_mode_t mode);
kl_logger_t *kl_logger_init_heap(FILE *file, kl_logger_mode_t mode);
int kl_logger_reset(kl_logger_t *lgr);
int kl_logger_destroy(kl_logger_t *lgr);

int kl_set_def_logger(const kl_logger_t *lgr);
kl_logger_t *kl_get_def_logger(void);


#endif
