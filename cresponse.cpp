#include "cresponse.h"
#include "sha1.h"
#include "base64.h"
#include <unistd.h>
#include <string.h>
#include <string>
#include <netinet/in.h>

CResponse::CResponse()
{

}

int CResponse::process(int fd, const char *buf, unsigned int len)
{

}

int CResponse::handshark(int fd, const char *buf, unsigned int len)
{
    std::string key(buf);
    key += MAGIC_KEY;
    SHA1 sha;
    unsigned int message_digest[5];
    sha.Reset();
    sha << key.c_str();
    sha.Result(message_digest);
    for (int i = 0; i < 5; i++)
        message_digest[i] = htonl(message_digest[i]);
    key = base64_encode(reinterpret_cast<const unsigned char*>(message_digest), 20);
    std::string msg;
    msg.append("HTTP/1.1 101 Switching Protocols\r\n");
    msg.append("Connection: Upgrade\r\n");
    msg.append("Sec-WebSocket-Accept: ");
    msg += (key + "\r\n");
    msg.append("Upgrade: websocket\r\n\r\n");
    write(STDOUT_FILENO, msg.c_str(), msg.size());
    return write(fd, msg.c_str(), msg.size());
}
