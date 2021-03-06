#include "crequest.h"
#include "cprotocol.h"
#include <unistd.h>
#include <errno.h>
#include <string.h>

CRequest::CRequest()
{
}

int CRequest::process(int fd, char *buf, unsigned int maxsize, bool b_conn)
{
    bzero(buf, maxsize);
    int nreads;
    if (b_conn) {
        while (true) {
            nreads = read(fd, buf, maxsize);
            if (nreads < 0 && errno == EINTR)
                continue;
            else
                break;
        }
        if (nreads > 0) {
            CProtocol cp;
            cp.decode(buf, nreads);
            if (cp.opcode() != 0x1)
                return -2;
            nreads = cp.payload_length();
            strncpy(buf, cp.payload_data(), nreads);
            buf[nreads] = '\0';
        }
        return nreads;
    } else {
        initbuf();
        while (*buf != '\r') {
            if ((nreads = readline(fd, buf, maxsize)) <= 0)
                return nreads;
            if (strncmp(buf, KEY_STRING, strlen(KEY_STRING)) == 0) {
                int pos = strlen(KEY_STRING) + 2;
                int len = nreads - pos - 2;
                strncpy(buf, buf + pos, len);
                buf[len] = '\0';
                return len;
            }
        }
        return -1;
    }

}

int CRequest::my_read(int fd, char *ptr)
{
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
    return 1;
}

int CRequest::readline(int fd, void *vptr, unsigned int maxlen)
{
    int n, rc;
    char c, *ptr;
    ptr = (char *)vptr;
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
            return -1;
        }
    }
    *ptr = 0;
    return n;
}

void CRequest::initbuf()
{
    bzero(read_buf, MAXLINE);
    read_cnt = 0;
    read_ptr = read_buf;
}

