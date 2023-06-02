#include <iostream>
#include <fstream>

#include "lib/structures.h"

#include "windows.h"

void RunWeatherForecast(Config config);

int main() {
    SetConsoleOutputCP(65001);

    std::ifstream config_file("config.json");
    Config config(json::parse(config_file));

    RunWeatherForecast(config);

    return 0;
}
