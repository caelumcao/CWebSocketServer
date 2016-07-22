#include <iostream>
#include "cserver.h"

int main(int argc, char *argv[])
{
    CServer *server = CServer::server();
    server->run();
    return 0;
}
