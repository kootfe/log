#ifndef LOG_H
#define LOG_H

#include <stdio.h>

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
    int isFile; //owns is too much key press so instead `ownsFile` its `isFile`
} KFTLogger;

int log(const KFTLogger *loger, const LogLevel level, const char *format, const char *fileinfo, const int line, const char *time, const char *date, ...);

#define KLOGF(loger, format, ...) log(loger, LOG, format, __FILE__, __LINE__, __TIME__, __DATE__, ##__VA_ARGS__);
int klogf(int isatty, FILE *logfile, const char *format, const char *file, const int line, const char *time, const char *date, va_list args);
#define ERRF(loger, format, ...) log(loger, ERROR, format, __FILE__, __LINE__, __TIME__, __DATE__, ##__VA_ARGS__);
int errf(int isatty, FILE *logfile, const char *format, const char *file, const int line, const char *time, const char *date, va_list args);
#define WARNF(loger, format, ...) log(loger, WARN, format, __FILE__, __LINE__, __TIME__, __DATE__, ##__VA_ARGS__);
int warnf(int isatty, FILE *logfile, const char *format, const char *file, const int line, const char *time, const char *date, va_list args);
#define SUCF(loger, format, ...) log(loger, SUC, format, __FILE__, __LINE__, __TIME__, __DATE__, ##__VA_ARGS__);
int sucf(int isatty, FILE *logfile, const char *format, const char *file, const int line, const char *time, const char *date, va_list args);

KFTLogger setup_kft_loger(FILE *file, LoggerModes mode);
void setup_debug_mode(const LoggerModes mode);
int clean_kft_loger(KFTLogger kftLogger);

#endif
