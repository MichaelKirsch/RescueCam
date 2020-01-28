#include <iostream>
#include <SFML/Graphics.hpp>
#include <experimental/filesystem>
#include "wiringPi.h"
#include "ThermalCamera.h"
#include <fstream>
#include <chrono>


void changeMode(int mode, sf::VertexArray& array,std::vector<float>& data, float tile_x,float tile_y)
{
    sf::Color bucol;

    for(int x =0;x<32;x++)
    {
        for(int y = 0;y<24;y++)
        {
            int pos = ((x*24)+y)*4;
            array[pos].position = sf::Vector2f(x*tile_x, y*tile_y);
            array[pos+1].position = sf::Vector2f((x+1)*tile_x, y*tile_y);
            array[pos+2].position = sf::Vector2f((x+1)*tile_x, (y+1)*tile_y);
            array[pos+3].position = sf::Vector2f(x*tile_x, (y+1)*tile_y);
            switch (mode)
            {
                case 0:
                    float low_end = 25.f;
                    float high_end = 40.f;
                    if(data[(x*24)+y]>low_end && data[(x*24)+y]<high_end)
                    {
                        float temp =data[(x*24)+y];
                        float delta = high_end-low_end;
                        temp-=low_end;
                        float perc = (1.f/delta)*temp;
                        if (perc<0.f)
                            perc=0.f;
                        bucol = {(sf::Uint8)(100+(154*perc)),(sf::Uint8)(100+(154*perc)),(sf::Uint8)(100+(154*perc))};
                    }

                    else
                        bucol = {(sf::Uint8)(0),(sf::Uint8)(0),(sf::Uint8)(0)};
                    break;
                case 1:
                    bucol = {(sf::Uint8)(rand()%255),(sf::Uint8)(rand()%255),(sf::Uint8)(rand()%255)};
                    break;
                case 2:
                    bucol = {(sf::Uint8)(rand()%100),(sf::Uint8)(rand()%100),(sf::Uint8)(rand()%100)};
                    break;
                default:
                    bucol = sf::Color::Magenta;
                    break;
            }
            array[pos].color = bucol;
            array[pos+1].color = bucol;
            array[pos+2].color = bucol;
            array[pos+3].color = bucol;
        }
    }
}


int main() {

    wiringPiSetup ();
    pinMode(0, INPUT);		// Configure GPIO0 as an output
    pinMode(2, INPUT);		// Configure GPIO1 as an input

    ThermalCamera camera(0x33,ThermalCamera::REFRESH_RATE::HZ_16);

    std::vector<float> data;
    data.reserve(800);
    sf::RenderWindow window;
    window.create(sf::VideoMode::getDesktopMode(),"SearchCam",sf::Style::Fullscreen);

    std::vector<std::string> modes;
    int mode = 0;
    modes.push_back("Mode: Life Detector");
    modes.push_back("Mode: Black equals Hot");
    modes.push_back("Mode: White equals Hot");
    modes.push_back("Mode: Temperature View");




    window.setMouseCursorVisible(false);
    float tilesize_x = window.getSize().x/32;
    float tilesize_y = window.getSize().y/24;
    sf::VertexArray cameraView(sf::Quads,32*24*4);

    sf::Clock cl;
    float timer =0;
    window.requestFocus();

    sf::Font font;
    std::string path_to_file = std::experimental::filesystem::current_path().string() + "/fonts/JetBrainsMono-Regular.ttf";
    font.loadFromFile("/home/pi/Rescuecam/RescueCam/fonts/JetBrainsMono-Regular.ttf");
    sf::Text text("Mode: Life Searcher",font,0.05*window.getSize().y);
    text.setFillColor(sf::Color::White);
    text.setPosition(window.getSize().x*0.03,window.getSize().y*0.03);

    float time_since_last_click = 0.f;

    while(window.isOpen())
    {
        auto delta_time =  cl.restart().asSeconds();
        timer+= delta_time;
        time_since_last_click+=delta_time;
        if(timer>1.f/16.f)
        {
            timer = 0.f;
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
            {
                window.close();
            }
            if(digitalRead(0)==0 && time_since_last_click>0.5f)
            {
                time_since_last_click =0.f;
                mode++;
                if(mode>modes.size()-1)
                    mode=0;
            }
            if(digitalRead(2)==0 && time_since_last_click>0.5f)
            {
                time_since_last_click =0.f;
                mode--;
                if(mode<0)
                    mode=modes.size()-1;

            }
            //text.setString(modes[mode]);
            camera.getFrame();
            if(camera.getSuccess())
            {
                data=camera.getTemps();
                text.setString(modes[mode]+"|"+std::to_string(data.size()));
            }
            else
            {
                text.setString(modes[mode]+"|"+"Fail");
                std::cout << "fail" << std::endl;
            }

            changeMode(mode,cameraView,data,tilesize_x,tilesize_y);
            window.clear();
            window.draw(cameraView);
            window.draw(text);
            window.display();
        }
        sf::Event ev;
        while(window.pollEvent(ev))
        {
            if(ev.type == sf::Event::Closed)
            {
                window.close();
            }
        }
    }
    return 0;
}
