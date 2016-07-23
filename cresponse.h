#ifndef CRESPONSE_H
#define CRESPONSE_H


class CResponse
{
public:
    CResponse();
    int process(int fd, char *buf, unsigned int len);
    int handshark(int fd, const char *buf, unsigned int len);

private:
    const char * MAGIC_KEY {"258EAFA5-E914-47DA-95CA-C5AB0DC85B11"};
};

#endif // CRESPONSE_H
