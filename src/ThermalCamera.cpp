

#include "ThermalCamera.h"

ThermalCamera::ThermalCamera() {
    wiringPiI2CSetup (1) ;
}

int ThermalCamera::readRegister(int reg) {
    return wiringPiI2CReadReg8 (adress,  reg) ;
}
