

#pragma once

#include "SFML/Graphics.hpp"
#include "wiringPi.h"
#include "ThermalCamera.h"
#include <opencv2/opencv.hpp>
#include <iostream>
#include <chrono>


class DisplayManager {
public:
    DisplayManager();
    void getNewFrame();
    void run();
    void updateDisplay(float elapsed);
    void updateCamera(float elapsed);
    void updateInputs(float elapsed);

    ~DisplayManager()
    {
        delete [] newPixel;
    };

private:
    sf::Uint8* newPixel;
    sf::Clock clock;
    sf::Text modeText;
    sf::Font font;
    sf::RectangleShape picture;
    sf::Image cameraImage, thermalImage;
    ThermalCamera camera;
    float normalCameraTimer=0.f;
    cv::Mat sfml_rgba_frame;
    cv::Mat cameraFrame;
    cv::VideoCapture stream1;
    bool overlay =true;
    float debounce_timer = 0.f;
    float display_timer,input_timer,camera_timer;
    unsigned int framerateCamera = 16;
    unsigned int framerateDisplay = 40;
    unsigned int framerateInput = 50;
    int mode = 0;
    float opening_angle = 68.f;
    int x_axis_correction = -1;
    int y_axis_correction = -5;
    std::array<float,768>* m_rawFrameData;
    sf::RenderWindow m_window;
    sf::Texture end_texture;
    sf::Clock timer;
protected:
};



