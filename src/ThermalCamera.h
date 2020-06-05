#pragma once


#include <vector>
#include <cstdint>
#include <array>
#include "MLX90640_API.h"
#include <cstring>

class ThermalCamera {
public:
    enum class REFRESH_RATE{
        HZ_0_5,HZ_1,HZ_2,HZ_4,HZ_8,HZ_16,HZ_32,HZ_64
    };
    ThermalCamera(uint8_t adress, REFRESH_RATE rr);
    int getRefreshRate();
    bool getFrame();
    bool getSuccess(){ return m_success;};
    float min_temp =0.f;
    float max_temp =0.f;
    std::array<float,768>& getTemps();
private:
    bool m_success;
    paramsMLX90640 mlx90640;
    std::array<float,768> m_temperatureVector;
    uint16_t eepromCamera[832];
    uint16_t frameData[834];

    std::array<float,768> image;
    uint16_t data[768* sizeof(float)];
    float temperatureArray[768];
    float eTa;
    float emissivity = 1;
    uint8_t m_adress;
    int subpage;

};



