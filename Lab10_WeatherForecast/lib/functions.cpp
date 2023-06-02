#include "conio.h"
#include "structures.h"

CoordResponse GetCoordinates(Config config) {
    cpr::Response response = cpr::Get(
            cpr::Url{"https://api.api-ninjas.com/v1/city?name=" + config.city_list[config.index]},
            cpr::Header{{"X-Api-Key", config.api_key_ninjas}});

    CoordResponse coord_response(response);
    return coord_response;
}

WeatherResponse GetWeather(const Config& config, const CoordResponse& coords) {
    std::string url = "https://api.open-meteo.com/v1/forecast?";
    url += "latitude=" + std::to_string(coords.latitude) + "&";
    url += "longitude=" + std::to_string(coords.longitude) + "&";
    url += "hourly=temperature_2m,weathercode,windspeed_10m,winddirection_10m,"
           "precipitation_probability,precipitation&current_weather=true&";
    url += "forecast_days=" + std::to_string(config.days) + "&";
    url += "timezone=auto";

    cpr::Response response = cpr::Get(cpr::Url{url});

    WeatherResponse weather(response);
    return weather;
}


void ClearScreen() {
#if defined _WIN32
    system("cls");
#elif defined (__LINUX__) || defined(__gnu_linux__) || defined(__linux__)
    system("clear");
    //std::cout<< u8"\033[2J\033[1;1H"; //Using ANSI Escape Sequences
#elif defined (__APPLE__)
    system("clear");
#endif
}

void CreateTable(WeatherResponse& weather);

void ReloadScreen(const Config& config) {
    CoordResponse coordinates = GetCoordinates(config);
    WeatherResponse weather;
    if (!coordinates.error) weather = GetWeather(config, coordinates);

    std::cout << std::flush;
    ClearScreen();
    std::cout << std::flush;
    std::cout << char(27) << "[3J";

    std::cout << ("Weather forecast: " + config.city_list[config.index] + "\n\n");

    if (weather.error) {
        std::cout << "Error! " << weather.error_reason << '\n';
        return;
    }
    if (coordinates.error) {
        std::cout << "Error! " << coordinates.error_reason << '\n';
        return;
    }
    CreateTable(weather);
}

void RunWeatherForecast(Config config) {
    enum BUTTONS {
        ESCAPE_KEY = 27,
        PLUS_KEY = '+', MINUS_KEY = '-',
        NEXT_KEY = 'n', PREV_KEY = 'p',
        NEXT_KEY_CAP = 'N', PREV_KEY_CAP = 'P'
    };

    while (true) {
        ReloadScreen(config);

        int INPUT_KEY = ' ';
        clock_t tstart = clock();
        while ((clock() - tstart) / CLOCKS_PER_SEC < config.reload_freq_sec) {
            if (kbhit()) {
                INPUT_KEY = getch();
                break;
            }
        }

        if (INPUT_KEY == BUTTONS::PLUS_KEY) {
            config.days++;
            if (config.days > 16) config.days = 16;
            if (config.days <= 0) config.days = 1;

        } else if (INPUT_KEY == BUTTONS::MINUS_KEY) {
            config.days--;
            if (config.days > 16) config.days = 16;
            if (config.days <= 0) config.days = 1;

        } else if (INPUT_KEY == BUTTONS::NEXT_KEY ||
                   INPUT_KEY == BUTTONS::NEXT_KEY_CAP) {
            config.index++;
            if (config.index == config.city_list.size()) config.index = 0;

        } else if (INPUT_KEY == BUTTONS::PREV_KEY ||
                   INPUT_KEY == BUTTONS::PREV_KEY_CAP) {
            config.index = (config.index > 0 ? config.index - 1 : (long long) config.city_list.size() - 1);
            std::cout << config.index;

        } else if (INPUT_KEY == BUTTONS::ESCAPE_KEY) {
            break;
        }
    }
}
