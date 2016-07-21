#ifndef CSERVER_H
#define CSERVER_H

class CLog;

class CServer
{
public:
    CServer();

    static CServer *server();
    void run();

private:
    void init();

private:
    static CServer *_server;
    CLog *_log;
};

#endif // CSERVER_H
