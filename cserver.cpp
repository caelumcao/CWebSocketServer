#include "cserver.h"

CServer *CServer::_server = nullptr;

CServer::CServer()
{

}

CServer *CServer::server()
{
    if (_server == nullptr)
        _server = new CServer;
    return _server;
}

