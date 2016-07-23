#include "cprotocol.h"
#include <memory.h>
#include <netinet/in.h>

CProtocol::CProtocol()
{
    init();
}

void CProtocol::encode(char *data, unsigned int &len)
{
    int pos = 0;
    memcpy(_payload_data, data, len);
    memset(data, 0, MAXLINE);
    memset(data, 0x81, 1);
    ++pos;
    if (len <= 125) {
        memset(data + pos, len, 1);
        ++pos;
    } else {

    }
    memcpy(data + pos, _payload_data, len);
    len = pos + len;
}

void CProtocol::decode(const char *msg, unsigned int len)
{
    int pos = 0;
    _fin = (unsigned char)msg[pos] >> 7;
    _opcode = msg[pos] & 0x0f;
    ++pos;
    _mask = (unsigned char)msg[pos] >> 7;
    _payload_length = msg[pos] & 0x7f;
    ++pos;
    if (_payload_length == 126) {
        u_int16_t length = 0;
        memcpy(&length, msg + pos, 2);
        pos += 2;
        _payload_length = ntohs(length);
    } else if (_payload_length == 127) {
        u_int32_t length = 0;
        memcpy(&length, msg + pos, 4);
        pos += 4;
        _payload_length = ntohl(length);
    }
    if (_mask == 1) {
        for (int i = 0; i < 4; ++i)
            _masking_key[i] = msg[pos + i];
        pos += 4;
    }
    if(_mask != 1){
        memcpy(_payload_data, msg + pos, _payload_length);
    }
    else {
        for(uint i = 0; i < _payload_length; ++i){
            int j = i % 4;
            _payload_data[i] = msg[pos + i] ^ _masking_key[j];
        }
    }
}

const char *CProtocol::payload_data()
{
    return _payload_data;
}

u_int64_t CProtocol::payload_length() const
{
    return _payload_length;
}

void CProtocol::init()
{
    _fin = 1;
    _opcode = 1;
    _mask = 0;
    memset(_masking_key, 0, sizeof(_masking_key));
    _payload_length = 0;
    memset(_payload_data, 0, sizeof(_payload_data));
}

u_int8_t CProtocol::opcode() const
{
    return _opcode;
}

