

#pragma once

#include <string>

bool getBit(int data, int position) // position in range 0-7
{
    return (data >> position) & 0x1;
}

std::string getBits16(int16_t data) {
    int length_in_bits = sizeof(data)*8;
    std::string buf;
    for(int x = 0;x<length_in_bits;x++)
    {
        buf+=std::to_string(getBit(data,x));
    }
    return buf;
}

std::string getBits8(int8_t data) {
    int length_in_bits = sizeof(data)*8;
    std::string buf;
    for(int x = 0;x<length_in_bits;x++)
    {
        buf+=std::to_string(getBit(data,x));
    }
    return buf;
}



