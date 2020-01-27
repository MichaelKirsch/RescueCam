

#include <stdint-gcc.h>
#include "ThermalCamera.h"

ThermalCamera::ThermalCamera() {
    wiringPiI2CSetup (1) ;
}

int8_t ThermalCamera::readRegister8(int reg) {
    return wiringPiI2CReadReg8 (adress,  reg) ;
}

int16_t ThermalCamera::readRegister16(int reg) {
    return wiringPiI2CReadReg16(adress,  reg) ;
}

std::string ThermalCamera::getBits(int data) {
    int length_in_bits = sizeof(data)*8;
    std::string buf;
    for(int x = 0;x<length_in_bits;x++)
    {
        buf+=std::to_string(getBit(data,x));
        buf+="|";
    }
    return buf;
}
