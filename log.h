#ifndef LOG_H
#define LOG_H
#include <stdio.h>
#include <stdarg.h>

typedef enum {
    ON,
    OFF,
    FORCE_OFF,
} LoggerModes;

typedef enum {
    ERROR,
    LOG,
    WARN,
    SUC,
} LogLevel;

typedef struct KFTLogger {
    FILE *logFile;
    LoggerModes mode;
    int isAtty;
    int ownsFile;
} KFTLogger;

int _klog(const KFTLogger *loger, const LogLevel level, const char *format, const char *fileinfo, const int line, const char *time, const char *date, ...);

//Take Log to some logger.
#define KLOGF(loger, format, ...) _klog(loger, LOG, format, __FILE__, __LINE__, __TIME__, __DATE__, ##__VA_ARGS__);
//Take Error to some logger.
#define ERRF(loger, format, ...) _klog(loger, ERROR, format, __FILE__, __LINE__, __TIME__, __DATE__, ##__VA_ARGS__);
//Take WARN to some logger.
#define WARNF(loger, format, ...) _klog(loger, WARN, format, __FILE__, __LINE__, __TIME__, __DATE__, ##__VA_ARGS__);
//Take Sucsses to some logger.
#define SUCF(loger, format, ...) _klog(loger, SUC, format, __FILE__, __LINE__, __TIME__, __DATE__, ##__VA_ARGS__);

/*
 * Return KFTLogger;
 * Setup the logger struct;
 */
KFTLogger setup_kft_loger(FILE *file, LoggerModes mode);
//Deprecad, If you use new versions.
void setup_debug_mode(const LoggerModes mode);
int clean_kft_loger(KFTLogger *kftLogger);

#endif
