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


KFTLogger setup_kft_loger(FILE *file, LoggerModes mode)
{
    KFTLogger loger;
    loger.logFile = file ? file : stderr;
    loger.mode = mode;
    loger.isAtty = isatty(fileno(loger.logFile));
    loger.ownsFile = (file != stderr && file != stdout && file != stdin);
    if (!loger.ownsFile) WARNF(&loger, "Log file is not set, using default stderr.\n");
    SUCF(&loger, "Seted the log system success fully\n");
    return loger;
}

int clean_kft_loger(KFTLogger *kftLogger)
{
    if (kftLogger->ownsFile) {
        fclose(kftLogger->logFile);
    }
    kftLogger->logFile = stderr;
    return 0;
}

int _print_log(int isatty, FILE *logfile, const char *format, const char *color, const char *level, const char *file, const int line, 
        const char *time, const char *date, va_list args)
{
    if (isatty)
        fprintf(logfile, "%s[%s] { %s:%d, - %s %s } ", color, level, file, line, time, date);
    else
        fprintf(logfile, "[%s] { %s:%d, - %s %s } ", level, file, line, time, date);
    vfprintf(logfile, format, args);
    if (isatty) fprintf(logfile, "%s", RESET);
    fflush(logfile);
    return 0;
}

int _klog(const KFTLogger *loger, const LogLevel level, const char *format, const char *fileinfo, const int line, const char *time, const char *date, ...)
{
    va_list args;
    va_start(args, date);
    FILE *logfile;
    LoggerModes mode;
    int isatty;
    if (!loger) {
        logfile = stderr;
        mode = ON;
        isatty = 1;
    } else {
        logfile = loger->logFile;
        mode = loger->mode;
        isatty = loger->isAtty;
    }

    switch (level) {
        case ERROR:
            if (mode == FORCE_OFF) break;
            _print_log(isatty, logfile, format, RED, "ERROR", fileinfo, line, time, date, args);
            break;
        case WARN:
            if (mode != ON) break;
            _print_log(isatty, logfile, format, YELLOW, "WARN", fileinfo, line, time, date, args);
            break;
        case LOG:
            if (mode != ON) break;
            _print_log(isatty, logfile, format, BLUE, "LOG", fileinfo, line, time, date, args);
            break;
        case SUC:
            if (mode != ON) break;
            _print_log(isatty, logfile, format, GREEN, "SUC", fileinfo, line, time, date, args);
            break;
    }

    va_end(args);
    return 0;
}

