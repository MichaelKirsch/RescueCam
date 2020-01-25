#include <iostream>
#include <SFML/Graphics.hpp>
#include <chrono>
int main() {
    sf::RenderWindow window;
    window.create(sf::VideoMode::getDesktopMode(),"SearchCam",sf::Style::Fullscreen);
    window.setMouseCursorVisible(false);
    sf::RectangleShape rect;
    rect.setSize({50,50});
    rect.setPosition({50,50});
    rect.setFillColor({200,0,0});
    std::vector<sf::RectangleShape> rects;

    float tilesize_x = window.getSize().x/32;
    float tilesize_y = window.getSize().y/24;
    sf::VertexArray cameraView(sf::Quads,32*24*4);
    int counter =0;
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
    while(window.isOpen())
    {
        timer+=cl.restart().asSeconds();
        if(timer>1.f/60.f)
        {
            timer = 0.f;
            window.clear();
            window.draw(cameraView);
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
