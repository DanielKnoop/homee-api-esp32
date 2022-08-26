#pragma once

#include <Arduino.h>

class MeasureBuffer : public Print
{
private:
    size_t _size = 0;

public:
    ~MeasureBuffer(){};

    size_t write(uint8_t c)
    {
        _size += 1;
        return 1;
    }
    size_t write(const uint8_t *buffer, size_t size)
    {
        _size += size;
        return size;
    }

    size_t size() { return _size; }
};