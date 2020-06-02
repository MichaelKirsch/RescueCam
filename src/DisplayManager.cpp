

#include "DisplayManager.h"

DisplayManager::DisplayManager():camera(0x33,ThermalCamera::REFRESH_RATE::HZ_16) {
    wiringPiSetup ();
    pinMode(0, INPUT);		// Configure GPIO0 as an output
    pinMode(2, INPUT);		// Configure GPIO1 as an input
    m_window.create(sf::VideoMode::getDesktopMode(),"RescueCam",sf::Style::Fullscreen);
    font.loadFromFile("data/fonts/JetBrainsMono-Regular.ttf");
    modeText.setFont(font);
    modeText.setPosition(m_window.getSize().x*0.04f,m_window.getSize().y*0.04f);
    modeText.setCharacterSize(m_window.getSize().y*0.03f);
    modeText.setFillColor(sf::Color::White);
    m_window.setMouseCursorVisible(false);
    cameraImage.create(640,480);
    thermalImage.create(32,24);
    picture.setSize({(float)m_window.getSize().x,(float)m_window.getSize().y});
    end_texture.loadFromFile("data/image.jpg");
    picture.setTexture(&end_texture);
}

void DisplayManager::updateInputs(float elapsed) {
    input_timer+=elapsed;
    debounce_timer +=elapsed;
    if(input_timer>1.f/framerateInput)
    {
        input_timer = 0.f;
        if(digitalRead(0)==0 && debounce_timer>0.5f)
        {
            debounce_timer = 0.f;
            //
        }
        if(digitalRead(2)==0 && debounce_timer>0.5f)
        {
            debounce_timer = 0.f;
            //
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
            m_window.close();

        sf::Event ev;
        while(m_window.pollEvent(ev)){}
    }

}

void DisplayManager::run() {
    while(m_window.isOpen())
    {
        //rt
        float elapsed = clock.restart().asSeconds();
        updateCamera(elapsed);
        updateDisplay(elapsed);
        updateInputs(elapsed);
    }
}

void DisplayManager::updateDisplay(float elapsed) {
    display_timer+=elapsed;

    if(display_timer>1.f/framerateDisplay)
    {
        display_timer = 0.f;
        m_window.clear();
        m_window.draw(picture);
        m_window.draw(modeText);
        m_window.display();
    }
}

void DisplayManager::updateCamera(float elapsed) {
    camera_timer += elapsed;
    if (camera_timer > 1.f / framerateCamera) {
        camera_timer = 0.f;
        int status = camera.getFrame();
        if (camera.getSuccess()) {
            m_rawFrameData = &camera.getTemps();
        }

        float lowest = 1000.f;
        float highest = -1000.f;
        for (auto &t:*m_rawFrameData) {
            if (t > highest)
                highest = t;
            if (t < lowest)
                lowest = t;
        }

        float temperatureRange = highest - lowest;
        auto& t = *m_rawFrameData;
        //thermalImage.create(32,24);
        //thermalImage.setPixel(5, 5, {254, 0, 0});
        thermalImage.loadFromFile("data/test.png");
        //for (int x = 0; x < 32; x++) {
        //    for (int y = 0; y < 24; y++) {
        //        int pos = ((x * 24) + y);
        //        float raw_temp = t[pos];
        //        raw_temp -= lowest;
        //        unsigned char processed_temp = (254.f / temperatureRange) * raw_temp;
        //        thermalImage.setPixel(x, y, {static_cast<unsigned char>(rand()%254), 0, 0});
        //    }
        //}
        //cameraImage.loadFromFile("data/image.jpg");
//
        //int x_small=0;
        //int y_small=0;
//
//
        //modeText.setString("Temperature Range:" + std::to_string(lowest)+"°C to "+std::to_string(highest)+"°C");
//
        //for(int x=0;x<cameraImage.getSize().x;x++)
        //    for(int y =0;y<cameraImage.getSize().y;y++)
        //    {
        //        sf::Color cameraColor = cameraImage.getPixel(x,y);
        //        x_small = x*(float)thermalImage.getSize().x/(float)cameraImage.getSize().x;
        //        y_small = y*(float)thermalImage.getSize().y/(float)cameraImage.getSize().y;
        //        sf::Color smallColor = thermalImage.getPixel(x_small,y_small);
        //        float factor = (1.0f/255.f)*smallColor.r;
//
//
        //        int t = (cameraColor.r+cameraColor.g+cameraColor.b)/3;
        //        float bright  =1.0f;
//
        //        int rest = bright*t;
//
        //        int red =t+(factor*255.f)*bright;
        //        if(red>=254)
        //            red= 254;
        //        if(rest>=254)
        //            rest = 254;
        //        sf::Color end(red,rest,rest);
        //        cameraImage.setPixel(x,y,end);
        //    }
        end_texture.loadFromImage(thermalImage);
    }
}