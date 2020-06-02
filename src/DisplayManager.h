

#pragma once

#include "SFML/Graphics.hpp"
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
    sf::RectangleShape picture;
    sf::Image cameraImage, thermalImage;
    ThermalCamera camera;

    float debounce_timer = 0.f;
    float display_timer,input_timer,camera_timer;
    unsigned int framerateCamera = 16;
    unsigned int framerateDisplay = 40;
    unsigned int framerateInput = 50;
    int mode = 0;

    std::array<float,768>* m_rawFrameData;
    sf::RenderWindow m_window;
    sf::Texture end_texture;

protected:
};



