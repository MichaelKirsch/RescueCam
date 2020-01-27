#pragma once

#include "wiringPiI2C.h"

class ThermalCamera {
public:
    ThermalCamera();
    int8_t readRegister8(int reg);
    int16_t readRegister16(int reg);
    ~ThermalCamera()= default;

private:
    const int adress = 0x33;
};



