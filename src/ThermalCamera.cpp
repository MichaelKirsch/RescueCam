


#include "ThermalCamera.h"


ThermalCamera::ThermalCamera(uint8_t adress,ThermalCamera::REFRESH_RATE rr ) {
    this->m_adress = adress;
    MLX90640_SetDeviceMode(m_adress,0);
    MLX90640_SetSubPageRepeat(m_adress,0);
    switch (rr)
    {
        case REFRESH_RATE::HZ_0_5:
            MLX90640_SetRefreshRate(m_adress, 0b000);
            break;
        case REFRESH_RATE::HZ_1:
            MLX90640_SetRefreshRate(m_adress, 0b001);
            break;
        case REFRESH_RATE::HZ_2:
            MLX90640_SetRefreshRate(m_adress, 0b010);
            break;
        case REFRESH_RATE::HZ_4:
            MLX90640_SetRefreshRate(m_adress, 0b011);
            break;
        case REFRESH_RATE::HZ_8:
            MLX90640_SetRefreshRate(m_adress, 0b100);
            break;
        case REFRESH_RATE::HZ_16:
            MLX90640_SetRefreshRate(m_adress, 0b101);
            break;
        case REFRESH_RATE::HZ_32:
            MLX90640_SetRefreshRate(m_adress, 0b110);
            break;
        case REFRESH_RATE::HZ_64:
            MLX90640_SetRefreshRate(m_adress, 0b111);
            break;
    }
    MLX90640_SetChessMode(m_adress);
    //MLX90640_SetInterleavedMode(m_adress);
    MLX90640_DumpEE(m_adress,eepromCamera);
    MLX90640_ExtractParameters(eepromCamera,&mlx90640);
}

int ThermalCamera::getRefreshRate() {
    return MLX90640_GetRefreshRate(m_adress);
}

bool ThermalCamera::getFrame() {
    m_success = false;
    int x = MLX90640_GetFrameData(m_adress, frameData);
    if(x==-1 || x == -8)
    {
        return false;
    }
    else
    {
        eTa = MLX90640_GetTa(frameData, &mlx90640);
        subpage = MLX90640_GetSubPageNumber(frameData);
        MLX90640_CalculateTo(frameData, &mlx90640, emissivity, eTa, temperatureArray);
        MLX90640_BadPixelsCorrection((&mlx90640)->brokenPixels, temperatureArray, 1, &mlx90640);
        MLX90640_BadPixelsCorrection((&mlx90640)->outlierPixels, temperatureArray, 1, &mlx90640);
        min_temp = 100.f;
        max_temp = -100.f;

        int counter =0;
            for(int x = 0; x < 32; x++) {
                for (int y = 0; y < 24; y++) {
                    float temp = temperatureArray[32 * (23 - y) + x];

                    if(temp>max_temp)
                        max_temp = temp;
                    if(temp<min_temp)
                        min_temp =temp;
                    m_temperatureVector[counter] = temp;
                    counter++;
                }
            }
        m_success = true;
        return true;
    }

}

std::array<float,768> &ThermalCamera::getTemps() {
    return m_temperatureVector;
}

