#include <iostream>
#include <SFML/Graphics.hpp>
#include <experimental/filesystem>
#include "wiringPi.h"
#include "ThermalCamera.h"
#include <fstream>
#include <chrono>
#include "DisplayManager.h"


int main() {
    DisplayManager manager;
    manager.run();
}
