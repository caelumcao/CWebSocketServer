#include "cserver.h"
#include "clog.h"

CServer *CServer::_server = nullptr;

CServer::CServer()
{
    _log = CLog::log();
}

CServer *CServer::server()
{
    if (_server == nullptr)
        _server = new CServer;
    return _server;
}

void CServer::run()
{
    init();
}

void CServer::init()
{

}

