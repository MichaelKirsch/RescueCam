#pragma once

#include "wiringPiI2C.h"
#include <string>

class ThermalCamera {
public:
    ThermalCamera();
    int8_t readRegister8(int reg);
    int16_t readRegister16(int reg);
    std::string getBits(int data);


    ~ThermalCamera()= default;

private:
    bool getBit(int data, int position) // position in range 0-7
    {
        return (data >> position) & 0x1;
    }
    const int adress = 0x33;
};



