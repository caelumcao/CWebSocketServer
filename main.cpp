#include <iostream>
#include "cserver.h"

#include "sha1.h"
#include "base64.h"
#include "netinet/in.h"

int main(int argc, char *argv[])
{
    CServer *server = CServer::server();
    server->run();

//    const char * MAGIC_KEY {"258EAFA5-E914-47DA-95CA-C5AB0DC85B11"};
//    std::string key("dGhlIHNhbXBsZSBub25jZQ==");
//    key += MAGIC_KEY;
//    SHA1 sha;
//    unsigned int message_digest[5];
//    sha.Reset();
//    sha << key.c_str();
//    sha.Result(message_digest);
//    for (int i = 0; i < 5; i++)
//        message_digest[i] = htonl(message_digest[i]);
//    key = base64_encode(reinterpret_cast<const unsigned char*>(message_digest), 20);
//    std::cout << key;

    return 0;
}
