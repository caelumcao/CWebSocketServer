#ifndef CREQUEST_H
#define CREQUEST_H


class CRequest
{
public:
    CRequest();
    int process(int fd, char *buf, unsigned int maxsize, bool b_conn);
private:
    enum {
        MAXLINE = 4096
    };
    const char *KEY_STRING {"Sec-WebSocket-Key"};
    int my_read(int fd, char *ptr);
    int readline(int fd, void *vptr, unsigned int maxlen);
};

#endif // CREQUEST_H
