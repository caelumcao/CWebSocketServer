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
    ssize_t my_read(int fd, char *ptr);
    ssize_t readline(int fd, void *vptr, size_t maxlen);
};

#endif // CREQUEST_H
