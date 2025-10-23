#define _POSIX_C_SOURCE 200809L
#include "headers/log.h"
#include <stdarg.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

static const char *RED = "\033[31m";
static const char *RESET = "\033[0m";
static const char *GREEN = "\033[32m";
static const char *YELLOW = "\x1b[33m"; //Just to anoy ppl couse its bigger then base10
static const char *BLUE = "\033[34m";

KFTLogger kftLogger = {
    .logFile = NULL,
    .mode = OFF,
    .isAtty = 1,
    .isFile = 0,
    .isFirst = 1,
};

void setup_debug_mode(const LoggerModes mode)
{
    kftLogger.mode = mode;
}

int setup_kft_loger(FILE *file)
{
    kftLogger.logFile = file;
    if (kftLogger.logFile == NULL) {
        ERRF("Cant Set Up The Log System!\n");
        return -1;
    }
    kftLogger.isAtty = isatty(fileno(kftLogger.logFile));
    kftLogger.isFile = (file != stderr && file != stdout && file != stdin);
    kftLogger.isFirst = 0;
    SUCF("Seted the log system success fully\n");
    return 0;
}

int clean_kft_loger() {
    if (!kftLogger.isFile) return -1;
    KLOGF("Truing to remove the log system...\n");
    if (kftLogger.logFile) {
        fclose(kftLogger.logFile);
        kftLogger.logFile = stderr;
        kftLogger.isAtty = isatty(fileno(kftLogger.logFile));
        kftLogger.isFile = 0;
        SUCF("Sucsess fully removed the log system | Closed File Descriptor | Nulliy it\n");
    } else {
        ERRF("Failed... File dont exist?!\n");
        return -1;
    }
    return 0;
}

int errf(const char *format, const char *file, const int line, const char *time, const char *date, ...)
{
    if (kftLogger.isFirst && !kftLogger.isFile) kftLogger.isFirst = 0, WARNF("Log file is not set, using default stderr.\n");
    if (kftLogger.mode == FORCE_OFF) return 0;
    va_list args;
    va_start(args, date);
    FILE *logf = kftLogger.logFile ? kftLogger.logFile : stderr;
    if (kftLogger.isAtty)
        fprintf(logf, "%s[ERROR] { %s:%d - %s %s } ", RED, file, line, time, date);
    else
        fprintf(logf, "[ERROR] { %s:%d - %s %s } ", file, line, time, date);
    vfprintf(logf, format, args);
    if (kftLogger.isAtty) fprintf(logf, "%s", RESET);
    va_end(args);
    fflush(logf);
    return 0;
}

int klogf(const char *format, const char *file, const int line, const char *time, const char *date, ...)
{
    if (kftLogger.isFirst && !kftLogger.isFile) kftLogger.isFirst = 0, WARNF("Log file is not set, using default stderr.\n");
    if (kftLogger.mode != ON) return 0;
    va_list args;
    va_start(args, date);
    FILE *logf = kftLogger.logFile ? kftLogger.logFile : stderr;
    if (kftLogger.isAtty)
        fprintf(logf, "%s[LOG] { %s:%d - %s %s } ", BLUE, file, line, time, date);
    else
        fprintf(logf, "[LOG] { %s:%d - %s %s } ", file, line, time, date);
    vfprintf(logf, format, args);
    if (kftLogger.isAtty) fprintf(logf, "%s", RESET);
    va_end(args);
    return 0;
}

int warnf(const char *format, const char *file, const int line, const char *time, const char *date, ...)
{
    if (kftLogger.isFirst && !kftLogger.isFile) kftLogger.isFirst = 0, WARNF("Log file is not set, using default stderr.\n");
    if (kftLogger.mode != ON) return 0;
    va_list args;
    va_start(args, date);

    FILE *logf = kftLogger.logFile ? kftLogger.logFile : stderr;
    if (kftLogger.isAtty)
        fprintf(logf, "%s[WARN] { %s:%d - %s %s } ", YELLOW, file, line, time, date);
    else 
        fprintf(logf, "[WARN] { %s:%d - %s %s } ", file, line, time, date);
    vfprintf(logf, format, args);
    if (kftLogger.isAtty) fprintf(logf, "%s", RESET);
    va_end(args);
    return 0;
}

int sucf(const char *format, const char *file, const int line, const char *time, const char *date, ...)
{
    if (kftLogger.isFirst && !kftLogger.isFile) kftLogger.isFirst = 0, WARNF("Log file is not set, using default stderr.\n");
    if (kftLogger.mode != ON) return 0;
    va_list args;
    va_start(args, date);

    FILE *logf = kftLogger.logFile ? kftLogger.logFile : stderr;
    if (kftLogger.isAtty)
        fprintf(logf, "%s[SUCCESS] { %s:%d - %s %s } ", GREEN, file, line, time, date);
    else
        fprintf(logf, "[SUCCESS] { %s:%d - %s %s } ", file, line, time, date);
    vfprintf(logf, format, args);
    if (kftLogger.isAtty) fprintf(logf, "%s", RESET);
    va_end(args);
    return 0;
}

//I dont have any better header to move this... bear with it i will move it... eventualy
void *kmalloc(size_t size, int *err)
{
    void *alloc = malloc(size);
    if (!alloc) {
        ERRF("Failed to Allocate(MALLOC)\n");
        *err = -1;
        return NULL;
    }
    *err = 0;
    return alloc;
}
