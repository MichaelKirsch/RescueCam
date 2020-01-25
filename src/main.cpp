#include <iostream>
#include <SFML/Graphics.hpp>
#include <experimental/filesystem>
#include <wiringPi.h>
#include <chrono>
int main() {

    wiringPiSetup ();
    pinMode(0, INPUT);		// Configure GPIO0 as an output
    pinMode(2, INPUT);		// Configure GPIO1 as an input
    sf::RenderWindow window;
    window.create(sf::VideoMode::getDesktopMode(),"SearchCam",sf::Style::Fullscreen);

    std::vector<std::string> modes;

    modes.push_back("Mode: Life Detector");
    modes.push_back("Mode: Black equals Hot");
    modes.push_back("Mode: White equals Hot");
    modes.push_back("Mode: Temperature View");


    window.setMouseCursorVisible(false);
    float tilesize_x = window.getSize().x/32;
    float tilesize_y = window.getSize().y/24;
    sf::VertexArray cameraView(sf::Quads,32*24*4);
    for(int x =0;x<32;x++)
    {
        for(int y = 0;y<24;y++)
        {
            int pos = ((x*24)+y)*4;
            cameraView[pos].position = sf::Vector2f(x*tilesize_x, y*tilesize_y);
            cameraView[pos+1].position = sf::Vector2f((x+1)*tilesize_x, y*tilesize_y);
            cameraView[pos+2].position = sf::Vector2f((x+1)*tilesize_x, (y+1)*tilesize_y);
            cameraView[pos+3].position = sf::Vector2f(x*tilesize_x, (y+1)*tilesize_y);
            sf::Color bucol = {(sf::Uint8)(rand()%255),(sf::Uint8)(rand()%255),(sf::Uint8)(rand()%255)};
            cameraView[pos].color = bucol;
            cameraView[pos+1].color = bucol;
            cameraView[pos+2].color = bucol;
            cameraView[pos+3].color = bucol;
        }
    }

    sf::Clock cl;
    float timer =0;
    window.requestFocus();

    sf::Font font;
    std::string path_to_file = std::experimental::filesystem::current_path().string() + "/fonts/JetBrainsMono-Regular.ttf";
    font.loadFromFile("/home/pi/RescueCam/fonts/JetBrainsMono-Regular.ttf");
    sf::Text text("Mode: Life Searcher",font,0.05*window.getSize().y);
    text.setFillColor(sf::Color::White);
    text.setPosition(0,0);

    float time_since_last_click = 0.f;
    int mode = 0;
    while(window.isOpen())
    {
        auto delta_time =  cl.restart().asSeconds();
        timer+= delta_time;
        time_since_last_click+=delta_time;
        if(timer>1.f/60.f)
        {
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
            {
                window.close();
            }
            if(digitalRead(0)==0 && time_since_last_click>0.5f)
            {
                time_since_last_click =0.f;
                mode++;
                if(mode>modes.size())
                    mode=0;
            }
            if(digitalRead(2)==0 && time_since_last_click>0.5f)
            {
                time_since_last_click =0.f;
                mode--;
                if(mode<0)
                    mode=modes.size()-1;
            }
            text.setString(modes[mode]);
            timer = 0.f;
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
