#ifndef CSERVER_H
#define CSERVER_H


class CServer
{
public:
    CServer();

    static CServer *server();

private:
    static CServer *_server;
};

#endif // CSERVER_H
