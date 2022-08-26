#pragma once
#include <Arduino.h>

class WriteBuffer : public Print
{
    public:
        ~WriteBuffer() {};
        WriteBuffer(uint8_t *buff, size_t len){ this->buff = buff; this->len = len; };

        size_t write(uint8_t value)
        {
            if (len == 0)
            {
                return 0;
            }
            *buff++ = value;
            --len;
            return 1;
        }

    private:
        size_t len = 0;
        uint8_t *buff;
};