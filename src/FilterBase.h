

#pragma once

#include "SFML/Graphics.hpp"
class FilterBase {
public:
    FilterBase()= default;
    std::vector<sf::Color> filter(std::vector<float>& to_filter);
    std::string getFilterInfo(){ return filter_info;};
    void nextFilter();
    void lastFilter();
    ~FilterBase()= default;
private:
    std::string filter_info;
    std::vector<sf::Color>life_finder(std::vector<float>& to_filter);
    std::vector<sf::Color>black_white(std::vector<float>& to_filter);
    std::vector<sf::Color>black_green(std::vector<float>& to_filter);
    std::vector<sf::Color>whole_spektrum(std::vector<float>& to_filter);
    int mode = 0;
protected:
};



