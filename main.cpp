#include <iostream>
#include <SFML/Graphics.hpp>
int main() {
    sf::RenderWindow window;
    window.create(sf::VideoMode(800,600),"penis");
    sf::RectangleShape rect;
    rect.setSize({50,50});
    rect.setPosition({50,50});
    rect.setFillColor({200,0,0});
    std::vector<sf::RectangleShape> rects;

    float tilesize_x = window.getSize().x/32;
    float tilesize_y = window.getSize().y/24;

    for(int x =0;x<32;x++)
    {
        for(int y = 0;y<24;y++)
        {
            //F
            sf::RectangleShape buffer;
            buffer.setSize({tilesize_x,tilesize_y});
            buffer.setPosition(x*tilesize_x,y*tilesize_y);
            buffer.setFillColor({(sf::Uint8)(rand()%255),(sf::Uint8)(rand()%255),(sf::Uint8)(rand()%255)});
            rects.push_back(buffer);
        }

    }




    sf::Clock cl;
    float time =0;
    while(window.isOpen())
    {
        time+=cl.restart().asSeconds();
        if(time>1.f/60.f)
        {
            time = 0.f;
            window.clear();
            for(auto el:rects)
            {
                window.draw(el);
            }
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
