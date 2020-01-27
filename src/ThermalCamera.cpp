

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
