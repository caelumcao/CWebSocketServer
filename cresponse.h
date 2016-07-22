#ifndef CRESPONSE_H
#define CRESPONSE_H


class CResponse
{
public:
    CResponse();
    int process(int fd, const char *buf, unsigned int len);
};

#endif // CRESPONSE_H
