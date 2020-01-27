#pragma once

#include "wiringPiI2C.h"

class ThermalCamera {
public:
    ThermalCamera();
    int readRegister(int reg);
    ~ThermalCamera()= default;

private:
    const int adress = 0x33;
};



