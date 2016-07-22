#ifndef CLOG_H
#define CLOG_H

#include <stdarg.h>

class CLog
{
public:
    CLog();
    void log_msg(int priority, const char *fmt, ...);
    void log_exit(int priority, const char *fmt, ...);

    static CLog *log();

private:
    void log_base(int priority, const char *fmt, va_list ap);

private:
    enum { BUFSIZE = 1024 };
    const char *LOGFILE {"./log"};

    static CLog *_log;
    int _fd;
    char buf[BUFSIZE];
};

#endif // CLOG_H
