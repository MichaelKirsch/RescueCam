

#include "DisplayManager.h"

DisplayManager::DisplayManager():camera(0x33,ThermalCamera::REFRESH_RATE::HZ_16) {
    wiringPiSetup ();
    pinMode(0, INPUT);		// Configure GPIO0 as an output
    pinMode(2, INPUT);		// Configure GPIO1 as an input
    m_rawFrameData.reserve(768);
    std::fill(m_rawFrameData.begin(),m_rawFrameData.end(),30.f);
    m_processedFrame.reserve(768);
    std::fill(m_processedFrame.begin(),m_processedFrame.end(),sf::Color::White);
    m_vertexArray.resize(32*24*4);
    m_vertexArray.setPrimitiveType(sf::Quads);
    m_window.create(sf::VideoMode::getDesktopMode(),"RescueCam",sf::Style::Fullscreen);
    font.loadFromFile("/home/pi/Rescuecam/RescueCam/fonts/JetBrainsMono-Regular.ttf");
    modeText.setFont(font);
    modeText.setPosition(m_window.getSize().x*0.04f,m_window.getSize().y*0.04f);
    modeText.setCharacterSize(m_window.getSize().y*0.04f);
    modeText.setFillColor(sf::Color::White);
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
            filter.nextFilter();
        }
        if(digitalRead(2)==0 && debounce_timer>0.5f)
        {
            debounce_timer = 0.f;
            filter.lastFilter();
        }
        sf::Event ev;
        while(m_window.pollEvent(ev)){}
    }

}

void DisplayManager::run() {
    while(true)
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
    float tile_x = m_window.getSize().x/32.f;
    float tile_y = m_window.getSize().y/24.f;
    int counter =0;
    if(display_timer>1.f/framerateDisplay)
    {
        for(int x =0;x<32;x++) {
            for (int y = 0; y < 24; y++) {
                int pos = ((x * 24) + y) * 4;
                m_vertexArray[pos].position = sf::Vector2f(x * tile_x, y * tile_y);
                m_vertexArray[pos + 1].position = sf::Vector2f((x + 1) * tile_x, y * tile_y);
                m_vertexArray[pos + 2].position = sf::Vector2f((x + 1) * tile_x, (y + 1) * tile_y);
                m_vertexArray[pos + 3].position = sf::Vector2f(x * tile_x, (y + 1) * tile_y);
                m_vertexArray[pos].color = m_processedFrame[counter];
                m_vertexArray[pos + 1].color = m_processedFrame[counter];
                m_vertexArray[pos + 2].color = m_processedFrame[counter];
                m_vertexArray[pos + 3].color = m_processedFrame[counter];
                counter++;
            }
        }
        display_timer = 0.f;
        modeText.setString(filter.getFilterInfo());
        m_window.clear();
        m_window.draw(m_vertexArray);
        m_window.draw(modeText);
        m_window.display();
    }
}

void DisplayManager::updateCamera(float elapsed) {
    camera_timer+=elapsed;
    if(camera_timer>1.f/framerateCamera)
    {
        camera_timer = 0.f;
        int status =  camera.getFrame();
        if(camera.getSuccess())
        {
            m_rawFrameData = camera.getTemps();
        }
        m_processedFrame = filter.filter(m_rawFrameData);
    }
}
