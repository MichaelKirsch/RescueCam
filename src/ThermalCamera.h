#pragma once

#include "wiringPiI2C.h"
#include <stdint-gcc.h>
#include <string>
#include <iostream>

class ThermalCamera {
public:
    ThermalCamera();
    int8_t readRegister8(int reg);
    int16_t readRegister16(int reg);
    std::string convertToString(int16_t value);
    std::string convertToString(int8_t value);
    ~ThermalCamera()= default;
    bool I2C_success(){ return m_I2C_success;};

private:
    bool m_I2C_success;
    int filehandle;
    const int adress = 0x33;
};



