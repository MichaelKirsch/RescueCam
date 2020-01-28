

#include "FilterBase.h"

std::vector<sf::Color> FilterBase::filter(std::vector<float> &to_filter) {
    switch(mode)
    {
        case 0:
            return life_finder(to_filter);
            break;
        case 1:
            return black_green(to_filter);
            break;
        case 2:
            return black_white(to_filter);
        case 3:
            return whole_spektrum(to_filter);
    }
}

std::vector<sf::Color> FilterBase::life_finder(std::vector<float> &to_filter) {
    std::vector<sf::Color> buffer;
    const float lower_end = 25.f;
    const float high_end = 40.f;
    float delta = high_end-lower_end;

    float lowest_temp =300.f;
    float highest_temp = -40.f;

    for(auto element:to_filter)
    {
        if(element<lowest_temp)
            lowest_temp=element;
        if(element>highest_temp)
            highest_temp=element;
        if(element>lower_end&&element<high_end)
        {
            float perc = 1.f/delta*(element-lowest_temp);
            if(perc<0.f)
                perc=0.f;
            buffer.emplace_back(sf::Color(100+(155*perc),0,0));
        } else
        {
            buffer.emplace_back(sf::Color::Black);
        }
    }
    filter_info = "Mode: Life-Finder ("+std::to_string(lower_end)+"|"+std::to_string(high_end)+") max:"+
            std::to_string(highest_temp)+" min:"+std::to_string(lowest_temp);
    return buffer;
}

std::vector<sf::Color> FilterBase::black_white(std::vector<float> &to_filter) {
    std::vector<sf::Color> buffer;

    float lowest_temp =300.f;
    float highest_temp = -40.f;
    for(auto element:to_filter)
    {
        if(element<lowest_temp)
            lowest_temp=element;
        if(element>highest_temp)
            highest_temp=element;
    }

    float delta = highest_temp-lowest_temp;

    for(auto element:to_filter)
    {
            float perc = 1.f/delta*(element-lowest_temp);
            buffer.emplace_back(sf::Color(255*perc,255*perc,255*perc));
    }
    filter_info = "Mode: Black White | max:" + std::to_string(highest_temp)+" min:"+std::to_string(lowest_temp);
    return buffer;
}

std::vector<sf::Color> FilterBase::black_green(std::vector<float> &to_filter) {
    std::vector<sf::Color> buffer;

    float lowest_temp =300.f;
    float highest_temp = -40.f;
    for(auto element:to_filter)
    {
        if(element<lowest_temp)
            lowest_temp=element;
        if(element>highest_temp)
            highest_temp=element;
    }

    float delta = highest_temp-lowest_temp;

    for(auto element:to_filter)
    {
        float perc = 1.f/delta*(element-lowest_temp);
        buffer.emplace_back(sf::Color(0*perc,255*perc,0*perc));
    }
    filter_info = "Mode: Black Green | max:" + std::to_string(highest_temp)+" min:"+std::to_string(lowest_temp);
    return buffer;
}

std::vector<sf::Color> FilterBase::whole_spektrum(std::vector<float> &to_filter) {
    std::vector<sf::Color> buffer;

    float lowest_temp =300.f;
    float highest_temp = -40.f;
    for(auto element:to_filter)
    {
        if(element<lowest_temp)
            lowest_temp=element;
        if(element>highest_temp)
            highest_temp=element;
    }

    float delta = highest_temp-lowest_temp;

    for(auto element:to_filter)
    {
        float perc = 1.f/delta*(element-lowest_temp);
        buffer.emplace_back(sf::Color(0*perc,255*perc,0*perc));
    }
    filter_info = "Mode: Spektrum | max:" + std::to_string(highest_temp)+" min:"+std::to_string(lowest_temp);
    return buffer;
}

void FilterBase::nextFilter() {
    if(mode<4)
        mode++;
    else
        mode=0;
}

void FilterBase::lastFilter() {
    if(mode>0)
        mode--;
    else
        mode=3;
}


