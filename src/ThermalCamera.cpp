


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
