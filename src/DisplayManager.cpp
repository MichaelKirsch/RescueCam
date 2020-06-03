

#include "DisplayManager.h"

DisplayManager::DisplayManager():camera(0x33,ThermalCamera::REFRESH_RATE::HZ_16) {

    if (!stream1.open(0)) {
        std::cout << "Camera failed to open" << std::endl;
    }

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
            opening_angle+=1.f;
        }
        if(digitalRead(2)==0 && debounce_timer>0.5f)
        {
            debounce_timer = 0.f;
            opening_angle-=1.f;
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

        float lowest = 25.f;
        float highest = -100.f;
        for (auto &t:*m_rawFrameData) {
            if (t > highest)
                highest = t;
        }



        float temperatureRange = highest - lowest;
        auto& t = *m_rawFrameData;

        for (int x = 0; x < 32; x++) {
            for (int y = 0; y < 24; y++) {
                int pos = ((x * 24) + y);
                float raw_temp = t[pos];
                if(raw_temp<=25.f)
                    raw_temp=lowest;
                raw_temp -= lowest;
                unsigned char processed_temp = (254.f / temperatureRange) * raw_temp;
                thermalImage.setPixel(x, y, {processed_temp, 0, 0});
            }
        }
        if(stream1.read(cameraFrame))
        {
            cv::cvtColor(cameraFrame, sfml_rgba_frame, cv::COLOR_BGR2BGRA);
            cameraImage.create(sfml_rgba_frame.cols, sfml_rgba_frame.rows,reinterpret_cast<sf::Uint8 *>(sfml_rgba_frame.ptr()));
        }
        int x_small=0;
        int y_small=0;

        float openingangleCamera = opening_angle;
        float openingAngleThermal = 55.f;
        sf::Vector2f resolutionThermal(32,24);
        sf::Vector2f resolutionNormal(640,480);

        float croppingfactor = (1.f/(0.5f*openingAngleThermal)*(0.5*openingangleCamera));
        int pixel_x = croppingfactor*resolutionThermal.x;
        int pixel_y = croppingfactor*resolutionThermal.y;
        int unused_x = resolutionThermal.x-pixel_x;
        int unused_y = resolutionThermal.y-pixel_y;

        float factor_x = (pixel_x/resolutionNormal.x);
        float factor_y = (pixel_y/resolutionNormal.y);

        modeText.setString("Temperature Range:" + std::to_string(lowest)+"°C to "+std::to_string(highest)+"°C Opening:" + std::to_string(opening_angle));

        for(int x=0;x<cameraImage.getSize().x;x++)
            for(int y =0;y<cameraImage.getSize().y;y++)
            {
                sf::Color cameraColor = cameraImage.getPixel(x,y);
                x_small = (unused_x/2)+(x*factor_x);
                y_small = (unused_y/2)+(y*factor_y);
                sf::Color smallColor = thermalImage.getPixel(x_small,y_small);
                float factor = (1.0f/255.f)*smallColor.r;
                int t = (cameraColor.r+cameraColor.g+cameraColor.b)/3;
                float bright  =0.7f;

                int rest = bright*t;

                int red =(t*bright)+smallColor.r;
                if(red>=254)
                    red= 254;
                if(rest>=254)
                    rest = 254;
                sf::Color end(red,rest,rest);
                cameraImage.setPixel(x,y,end);

            }
        end_texture.loadFromImage(cameraImage);

    }
}