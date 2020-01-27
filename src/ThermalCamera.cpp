


#include "ThermalCamera.h"

ThermalCamera::ThermalCamera() {
    filehandle = wiringPiI2CSetup (adress);
    if(filehandle ==-1)
    {
        m_I2C_success = false;
        std::cout << "fuck, cant init the camera" << std::endl;
    } else
    {
        m_I2C_success = true;
    }
}

int8_t ThermalCamera::readRegister8(int reg) {
    if(m_I2C_success)
        return wiringPiI2CReadReg8 (filehandle,  reg) ;
    else
        return 0;
}

int16_t ThermalCamera::readRegister16(int reg) {
    return wiringPiI2CReadReg16(filehandle,  reg) ;
}

std::string ThermalCamera::convertToString(int16_t value) {
    std::string buf;
    // continously divide the integer by 2, if there is no remainder, the bit is 1, else it's 0
    for (int i = (sizeof(value)*8)-1; i >= 0 ; i--)
    {
        int16_t mask = 0;
        mask = 1 << i;
        std::cout << mask << std::endl;
        if((value & mask) == mask)
            buf+=std::to_string(1);
        else
            buf+=std::to_string(0);
    }
    return buf;
}

std::string ThermalCamera::convertToString(int8_t value) {
    std::string buf;
    // continously divide the integer by 2, if there is no remainder, the bit is 1, else it's 0
    for (int i = (sizeof(value)*8)-1; i >= 0 ; i--)
    {
        int16_t mask = 0;
        mask = 1 << i;
        std::cout << mask << std::endl;
        if((value & mask) == mask)
            buf+=std::to_string(1);
        else
            buf+=std::to_string(0);
    }
    return buf;
}

std::string ThermalCamera::convertToString(int value) {
    std::string buf;
    // continously divide the integer by 2, if there is no remainder, the bit is 1, else it's 0
    for (int i = (sizeof(value)*8)-1; i >= 0 ; i--)
    {
        int16_t mask = 0;
        mask = 1 << i;
        std::cout << mask << std::endl;
        if((value & mask) == mask)
            buf+=std::to_string(1);
        else
            buf+=std::to_string(0);
    }
    return buf;
}

