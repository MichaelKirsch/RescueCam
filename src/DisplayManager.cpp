

#include "DisplayManager.h"

DisplayManager::DisplayManager():camera(0x33,ThermalCamera::REFRESH_RATE::HZ_16) {

    if (!stream1.open(0)) {
        std::cout << "Camera failed to open" << std::endl;
    }

    wiringPiSetup ();
    pinMode(0, INPUT);		// Configure GPIO0 as an output
    pinMode(2, INPUT);		// Configure GPIO1 as an input
    m_window.create(sf::VideoMode::getDesktopMode(),"RescueCam",sf::Style::Fullscreen);
    font.loadFromFile("/home/pi/RescueCam/build/data/fonts/JetBrainsMono-Regular.ttf");
    modeText.setFont(font);
    modeText.setPosition(m_window.getSize().x*0.04f,m_window.getSize().y*0.04f);
    modeText.setCharacterSize(m_window.getSize().y*0.03f);
    modeText.setFillColor(sf::Color::White);
    m_window.setMouseCursorVisible(false);
    cameraImage.create(640,480);
    thermalImage.create(32,24);
    picture.setSize({(float)m_window.getSize().x,(float)m_window.getSize().y});
    end_texture.loadFromFile("/home/pi/RescueCam/build/data/image.jpg");
    picture.setTexture(&end_texture);
    modeText.setFillColor(sf::Color::Green);
    newPixel =new sf::Uint8[cameraImage.getSize().x*cameraImage.getSize().y*4];

}

void DisplayManager::updateInputs(float elapsed) {
    input_timer+=elapsed;
    debounce_timer +=elapsed;
    if(input_timer>1.f/framerateInput)
    {
        input_timer = 0.f;
        if(digitalRead(0)==0 && debounce_timer>0.5f)
        {
            const auto p1 = std::chrono::system_clock::now();
            cameraImage.saveToFile("/home/pi/RescueCam/build/data/"+std::to_string(std::chrono::duration_cast<std::chrono::seconds>(
                    p1.time_since_epoch()).count())+".jpg");
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
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
            y_axis_correction+=1;
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
            y_axis_correction-=1;
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
            x_axis_correction+=1;
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
            x_axis_correction-=1;
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::M))
            opening_angle+=1.f;
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::N))
            opening_angle-=1.f;
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
    std::cout << "x_axis_cor:" << x_axis_correction << " y_axis_cor:" << y_axis_correction << " Angle:" << opening_angle<< std::endl;


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
    normalCameraTimer+=elapsed;
    if (normalCameraTimer > 1.f / 30) {
        clock.restart();
        normalCameraTimer = 0.f;
        if (stream1.read(cameraFrame)) {
            cv::cvtColor(cameraFrame, sfml_rgba_frame, cv::COLOR_BGR2BGRA);
            cameraImage.create(640, 480, reinterpret_cast<sf::Uint8 *>(sfml_rgba_frame.ptr()));

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
            sf::Color cameraColor,smallColor,end;
            int red=0;
            int t=0;
            float factor=0.f;
            for(int x=0;x<cameraImage.getSize().x;x+=1)
            {
                for(int y =0;y<cameraImage.getSize().y;y+=1)
                {
                    x_small = (unused_x/2)+x_axis_correction+(x*factor_x);
                    y_small = (unused_y/2)+y_axis_correction+(y*factor_y);
                    smallColor = thermalImage.getPixel(x_small,y_small);
                    if(smallColor.r>50)
                    {
                        cameraColor = cameraImage.getPixel(x,y);;
                        t = (cameraColor.r+cameraColor.g+cameraColor.b)/3;
                        red =t+smallColor.r;
                        if(red>=254)
                            red=254;
                        cameraImage.setPixel(x,y,sf::Color(red,t,t));
                    }
                }
            }
            end_texture.update(cameraImage);
            modeText.setString("Timer:" +std::to_string(clock.restart().asMilliseconds()) + " MAX:" + std::to_string(camera.max_temp)+ " MIN:" + std::to_string(camera.min_temp));
        }
    }
    if (camera_timer > 1.f / framerateCamera) {

        camera_timer = 0.f;
        int status = camera.getFrame();
        if (camera.getSuccess()) {
            m_rawFrameData = &camera.getTemps();

            float temperatureRange =camera.max_temp-camera.min_temp;
            auto& t = *m_rawFrameData;

            for (int x = 0; x < 32; x++) {
                for (int y = 0; y < 24; y++) {
                    int pos = ((x * 24) + y);
                    float raw_temp = t[pos];
                    if (raw_temp <= 25.f)
                        raw_temp = camera.min_temp;
                    raw_temp -= camera.min_temp;
                    unsigned char processed_temp =0;
                    if(raw_temp>10)
                        processed_temp = (255.f / temperatureRange) * raw_temp;
                    thermalImage.setPixel(x, y, {processed_temp, 0, 0});
                }
            }
        }
    }
}