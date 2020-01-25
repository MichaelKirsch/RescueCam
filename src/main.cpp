#include <iostream>
#include <SFML/Graphics.hpp>
#include <string>
#include <experimental/filesystem>
#include <chrono>
int main() {
    sf::RenderWindow window;
    window.create(sf::VideoMode::getDesktopMode(),"SearchCam");
    window.setMouseCursorVisible(false);
    sf::Text text;
    sf::Font font;
    //ffff
    std::string path_exec = std::experimental::filesystem::current_path().string();
    std::cout << path_exec << std::endl;
    auto path_font = path_exec + "/fonts/JetBrainsMono-Regular.ttf";
    font.loadFromFile(path_font);
    text.setCharacterSize(0.05f*window.getSize().y);
    text.setFillColor(sf::Color::White);
    text.setPosition(0,0);
    text.setString("Mode: Life-Search");
    text.setFont(font);
    sf::RectangleShape rect;
    rect.setSize({50,50});
    rect.setPosition({50,50});
    rect.setFillColor({200,0,0});
    std::vector<sf::RectangleShape> rects;
    //hier
    float tilesize_x = window.getSize().x/32;
    float tilesize_y = window.getSize().y/24;

    for(int x =0;x<32;x++)
    {
        for(int y = 0;y<24;y++)
        {
            sf::RectangleShape buffer;
            buffer.setSize({tilesize_x,tilesize_y});
            buffer.setPosition(x*tilesize_x,y*tilesize_y);
            buffer.setFillColor({(sf::Uint8)(rand()%255),(sf::Uint8)(rand()%255),(sf::Uint8)(rand()%255)});
            rects.push_back(buffer);
        }

    }
    sf::Clock cl;
    float timer =0;
    window.requestFocus();
    while(window.isOpen())
    {
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
        {
            window.close();
        }
        timer+=cl.restart().asSeconds();
        if(timer>1.f/60.f)
        {
            timer = 0.f;
            window.clear();
            for(auto el:rects)
            {
                window.draw(el);
            }
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
