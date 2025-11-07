#define _POSIX_C_SOURCE 200809L
#include "log.h"
#include <stdarg.h>
#include <stdio.h>
#include <unistd.h>

static const char *RED = "\033[31m";
static const char *RESET = "\033[0m";
static const char *GREEN = "\033[32m";
static const char *YELLOW = "\x1b[33m"; //Just to anoy ppl couse its bigger then base10
static const char *BLUE = "\033[34m";


KFTLogger setup_kft_loger(LogFileUnion file, LoggerTypes loggertype, LoggerModes mode, size_t loggerCount)
{
    KFTLogger loger;
    switch (loggertype) {
        case SingleFileLogger:
            loger = setup_kft_loger_single_file(file.logFile.file, mode);
            break;
        case MultiFileLogger:
            loger = setup_kft_loger_multi_file(file.logFiles, loggerCount, mode);
            break;
    }
    return loger;
}

KFTLogger setup_kft_loger_single_file(FILE *file, LoggerModes mode)
{
    KFTLogger loger;
    loger.file.logFile.file = file ? file : stderr;
    loger.mode = mode;
    loger.file.logFile.isatty = isatty(fileno(loger.file.logFile.file));
    loger.isFile = (file != stderr && file != stdout && file != stdin);
    if (!loger.isFile) WARNF(&loger, "Log file is not set, using default stderr.\n");
    SUCF(&loger, "Seted the log system success fully\n");
    return loger;
}

KFTLogger setup_kft_loger_multi_file(LogFile files[], size_t len, LoggerModes mode)
{
    KFTLogger loger;
    loger.file.logFiles = files;
    for (size_t i = 0; len; ++i) {
        loger.file.logFiles[i].file = (files[i].file != NULL ? files[i].file : stderr);
        loger.file.logFiles[i].isatty = isatty(fileno(loger.file.logFiles[i].file));
    }
    loger.mode = mode;
    SUCF(&loger, "Seted the log system success fully\n");
    return loger;
}

/*
int clean_kft_loger(KFTLogger kftLogger)
{
    if (kftLogger.isFile) {
        fclose(kftLogger.logFile);
    }
    kftLogger.logFile = stderr;
    return 0;
}

int klog(const KFTLogger *loger, const LogLevel level, const char *format, const char *fileinfo, const int line, const char *time, const char *date, ...)
{
    va_list args;
    va_start(args, date);
    FILE *logfile;
    LoggerModes mode;
    int isatty;
    if (!loger) {
        logfile = stderr;
        mode = OFF;
        isatty = 1;
    } else {
        logfile = loger->logFile;
        mode = loger->mode;
        isatty = loger->isAtty;
    }

    switch (level) {
        case ERROR:
            if (mode == FORCE_OFF) break;
            errf(isatty, logfile, format, fileinfo, line, time, date, args);
            break;
        case WARN:
            if (mode != ON) break;
            warnf(isatty, logfile, format, fileinfo, line, time, date, args);
            break;
        case LOG:
            if (mode != ON) break;
            klogf(isatty, logfile, format, fileinfo, line, time, date, args);
            break;
        case SUC:
            if (mode != ON) break;
            sucf(isatty, logfile, format, fileinfo, line, time, date, args);
            break;
    }

    va_end(args);
    return 0;
}

int errf(int isatty, FILE *logfile, const char *format, const char *file, const int line, const char *time, const char *date, va_list args)
{
    
    if (isatty)
        fprintf(logfile, "%s[ERROR] { %s:%d - %s %s } ", RED, file, line, time, date);
    else
        fprintf(logfile, "[ERROR] { %s:%d - %s %s } ", file, line, time, date);
    vfprintf(logfile, format, args);
    if (isatty) fprintf(logfile, "%s", RESET);
    fflush(logfile);
    return 0;
}

int klogf(int isatty, FILE *logfile, const char *format, const char *file, const int line, const char *time, const char *date, va_list args)
{
    if (isatty)
        fprintf(logfile, "%s[LOG] { %s:%d - %s %s } ", BLUE, file, line, time, date);
    else
        fprintf(logfile, "[LOG] { %s:%d - %s %s } ", file, line, time, date);
    vfprintf(logfile, format, args);
    if (isatty) fprintf(logfile, "%s", RESET);
    return 0;
}

int warnf(int isatty, FILE *logfile, const char *format, const char *file, const int line, const char *time, const char *date, va_list args)
{
    if (isatty)
        fprintf(logfile, "%s[WARN] { %s:%d - %s %s } ", YELLOW, file, line, time, date);
    else 
        fprintf(logfile, "[WARN] { %s:%d - %s %s } ", file, line, time, date);
    vfprintf(logfile, format, args);
    if (isatty) fprintf(logfile, "%s", RESET);
    return 0;
}

int sucf(int isatty, FILE *logfile, const char *format, const char *file, const int line, const char *time, const char *date, va_list args)
{
    if (isatty)
        fprintf(logfile, "%s[SUCCESS] { %s:%d - %s %s } ", GREEN, file, line, time, date);
    else
        fprintf(logfile, "[SUCCESS] { %s:%d - %s %s } ", file, line, time, date);
    vfprintf(logfile, format, args);
    if (isatty) fprintf(logfile, "%s", RESET);
    return 0;
}
*/
