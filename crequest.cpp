#include "crequest.h"
#include <unistd.h>
#include <errno.h>
#include <string>
#include <sstream>
CRequest::CRequest()
{

}

int CRequest::process(int fd, char *buf, unsigned int maxsize, bool b_conn)
{
    int nreads;
    while (true) {
        nreads = read(fd, buf, maxsize);
        if (nreads < 0 && errno == EINTR)
            continue;
        else
            break;
    }

    if (b_conn) {

    } else {

    }
    return nreads;
}

ssize_t CRequest::my_read(int fd, char *ptr)
{
    static int read_cnt = 0;
    static char *read_ptr;
    static char read_buf[MAXLINE];
    if (read_cnt <= 0) {
        while (true) {
            if ((read_cnt = read(fd, read_buf, sizeof(read_buf))) < 0) {
                if (errno == EINTR)
                    continue;
                return -1;
            } else if (read_cnt == 0) {
                return 0;
            } else {
                read_ptr = read_buf;
                break;
            }
        }
    }
    read_cnt--;
    *ptr = *read_ptr++;
    return(1);
}

ssize_t CRequest::readline(int fd, void *vptr, size_t maxlen)
{
    int n, rc;
    char c, *ptr;
    ptr = vptr;
    for (n = 1; n < maxlen; ++n) {
        if ((rc = my_read(fd, &c)) == 1) {
            *ptr++ = c;
            if (c == '\n')
                break;
        } else if (rc == 0) {
            if (n == 1)
                return 0;
            else
                break;
        } else {
            return(-1);
        }
    }
    *ptr = 0;
    return n;
}

