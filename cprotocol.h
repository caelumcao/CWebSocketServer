#ifndef CPROTOCOL_H
#define CPROTOCOL_H

#include <unistd.h>
#include <sys/types.h>

class CProtocol
{
public:
    CProtocol();
    void encode(char *data, unsigned int &len);
    void decode(const char *msg, unsigned int len);
    const char *payload_data();
    u_int64_t payload_length() const;
    u_int8_t opcode() const;

private:
    void init();

private:
    enum {
        MAXLINE = 4096
    };
    u_int8_t _fin;
    u_int8_t _opcode;
    u_int8_t _mask;
    u_int8_t _masking_key[4];
    u_int64_t _payload_length;
    char _payload_data[MAXLINE];
};

#endif // CPROTOCOL_H
