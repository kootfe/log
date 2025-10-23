#ifndef LOG_H
#define LOG_H

#include <stdio.h>

typedef enum {
    ON,
    OFF,
    FORCE_OFF,
} LoggerModes;

typedef struct KFTLogger {
    FILE *logFile;
    LoggerModes mode;
    int isAtty;
    int isFile; //owns is too much key press so instead `ownsFile` its `isFile`
    int isFirst;
} KFTLogger;

extern KFTLogger kftLogger;

#define KLOGF(format, ...) klogf(format, __FILE__, __LINE__, __TIME__, __DATE__, ##__VA_ARGS__)
int klogf(const char *format, const char *file, const int line, const char *time, const char *date, ...);
#define ERRF(format, ...) errf(format, __FILE__, __LINE__, __TIME__, __DATE__, ##__VA_ARGS__)
int errf(const char *format, const char *file, const int line, const char *time, const char *date, ...);
#define WARNF(format, ...) warnf(format, __FILE__, __LINE__, __TIME__, __DATE__, ##__VA_ARGS__)
int warnf(const char *format, const char *file, const int line, const char *time, const char *date, ...);
#define SUCF(format, ...) sucf(format, __FILE__, __LINE__, __TIME__, __DATE__, ##__VA_ARGS__)
int sucf(const char *format, const char *file, const int line, const char *time, const char *date, ...);

void setup_debug_mode(const LoggerModes mode);
int setup_kft_loger(FILE *file);
int clean_kft_loger();

void *kmalloc(size_t size, int *err);

#endif
