

#pragma once

#include "SFML/Graphics.hpp"
#include "FilterBase.h"
#include "wiringPi.h"
#include "ThermalCamera.h"
class DisplayManager {
public:
    DisplayManager();
    void getNewFrame();
    void run();
    void updateDisplay(float elapsed);
    void updateCamera(float elapsed);
    void updateInputs(float elapsed);

    ~DisplayManager()=default;

private:
    sf::Clock clock;
    sf::Text modeText;
    sf::Font font;

    FilterBase filter;
    ThermalCamera camera;

    float debounce_timer = 0.f;
    float display_timer,input_timer,camera_timer;
    unsigned int framerateCamera = 16;
    unsigned int framerateDisplay = 40;
    unsigned int framerateInput = 50;
    int mode = 0;

    std::vector<float> m_rawFrameData;
    std::vector<sf::Color> m_processedFrame;
    sf::VertexArray m_vertexArray;
    sf::RenderWindow m_window;
protected:
};



