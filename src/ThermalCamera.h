#pragma once

#include "mlx90640/MLX90640_API.h"
#include <vector>
#include <cstdint>
#include <cstring>

class ThermalCamera {
public:
    enum class REFRESH_RATE{
        HZ_0_5,HZ_1,HZ_2,HZ_4,HZ_8,HZ_16,HZ_32,HZ_64
    };
    ThermalCamera(uint8_t adress, REFRESH_RATE rr);
    int getRefreshRate();
    void getFrame();
    std::vector<float>& getTemps();


private:
    paramsMLX90640 mlx90640;
    std::vector<float> m_temperatureVector;
    uint16_t eepromCamera[832];
    uint16_t frameData[834];
    float image[768];
    uint16_t data[768* sizeof(float)];
    float temperatureArray[768];

    float eTa;
    float emissivity = 1;
    uint8_t m_adress;
    int subpage;

};



