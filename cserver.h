#ifndef CSERVER_H
#define CSERVER_H

#include "crequest.h"
#include "cresponse.h"
#include <list>
#include <map>

class CLog;

class CServer
{
public:
    CServer();

    static CServer *server();
    void run();

private:
    void init();
    void do_epoll();
    void add_event(int fd);
    void delete_event(int fd);

private:
    enum {
        EPOLLSIZE = 1024,
        SERVPORT = 9999,
        MAXLINE = 4096
         };
    static CServer *_server;
    CLog *_log;
    CRequest _request;
    CResponse _response;
    int _listenfd;
    int _epollfd;
    std::list<int> _fdlist;
    std::map<int, bool> _b_connmap;
};


#endif // CSERVER_H
