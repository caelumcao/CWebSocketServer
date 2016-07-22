#include "clog.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>


CLog *CLog::_log = nullptr;

CLog::CLog()
{
    _fd = open(LOGFILE, O_WRONLY | O_APPEND | O_CREAT, 0644);
    if (_fd < 0) {
        perror("log open failed");
        exit(1);
    }
}

void CLog::log_msg(int priority, const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    log_base(priority, fmt, ap);
    va_end(ap);
}

void CLog::log_exit(int priority, const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    log_base(priority, fmt, ap);
    va_end(ap);
    exit(1);
}

CLog *CLog::log()
{
    if (_log == nullptr)
        _log = new CLog;
    return _log;
}

void CLog::log_base(int priority, const char *fmt, va_list ap)
{
    time_t t;
    struct tm *tmp;
    time(&t);
    tmp = localtime(&t);
    snprintf(buf, BUFSIZE, "[%d]", priority);
    strftime(buf + strlen(buf), BUFSIZE - strlen(buf), "[%F %X]", tmp);
    vsnprintf(buf + strlen(buf), BUFSIZE - strlen(buf), fmt, ap);
    snprintf(buf + strlen(buf), 2, "\n");
    if (write(_fd, buf, strlen(buf)) < 0) {
        perror("log_msg write failed");
        exit(1);
    }
}
