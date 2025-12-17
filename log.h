#ifndef LOG_H
#define LOG_H
#include <stdio.h>
#include <stdarg.h>

typedef enum kl_logger_mode_t {
    ON = 0,
    OFF = 1,
    FORCE_OFF = 2,
} kl_logger_mode_t;

typedef enum kl_log_level_t {
    ERROR,
    LOG,
    WARN,
    SUC,
} kl_log_level_t;

typedef struct kl_logger kl_logger_t;
int kl_log_impl(const kl_logger_t *loger, const kl_log_level_t level, const char *format, const char *fileinfo, const int line, const char *time, const char *date, ...);

#define klog(format, ...) kl_log_impl(kl_get_def_logger(), LOG, format, __FILE__, __LINE__, __TIME__, __DATE__, ##__VA_ARGS__)
#define klog_sp(loger, format, ...) kl_log_impl(loger, LOG, format, __FILE__, __LINE__, __TIME__, __DATE__, ##__VA_ARGS__)
#define errf(format, ...) kl_log_impl(kl_get_def_logger(), ERROR, format, __FILE__, __LINE__, __TIME__, __DATE__, ##__VA_ARGS__)
#define errf_sp(loger, format, ...) kl_log_impl(loger, ERROR, format, __FILE__, __LINE__, __TIME__, __DATE__, ##__VA_ARGS__)
#define warnf(format, ...) kl_log_impl(kl_get_def_logger(), WARN, format, __FILE__, __LINE__, __TIME__, __DATE__, ##__VA_ARGS__)
#define warnf_sp(loger, format, ...) kl_log_impl(loger, WARN, format, __FILE__, __LINE__, __TIME__, __DATE__, ##__VA_ARGS__)
#define sucf(format, ...) kl_log_impl(kl_get_def_logger(), SUC, format, __FILE__, __LINE__, __TIME__, __DATE__, ##__VA_ARGS__)
#define sucf_sp(loger, format, ...) kl_log_impl(loger, SUC, format, __FILE__, __LINE__, __TIME__, __DATE__, ##__VA_ARGS__)

int kl_set_mode(kl_logger_t *lgr, kl_logger_mode_t mode);
kl_logger_mode_t kl_get_mode(const kl_logger_t *logger);
int kl_set_file(kl_logger_t *lgr, FILE *file);
FILE *kl_get_file(const kl_logger_t *logger);

kl_logger_t kl_logger_init(FILE *file, kl_logger_mode_t mode);
int kl_logger_reset(kl_logger_t *lgr);

int kl_set_def_logger(const kl_logger_t *lgr);
kl_logger_t *kl_get_def_logger(void);


#endif
