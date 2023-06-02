#ifndef LABWORK10_STRUCTURES_H
#define LABWORK10_STRUCTURES_H

#include <iostream>

#include "cpr/cpr.h"

#include "nlohmann/json.hpp"
using json = nlohmann::json;


struct Config {
    std::string api_key_ninjas;
    std::vector<std::string> city_list;
    long long days;
    size_t reload_freq_sec;
    long long index = 0;

    explicit Config(json config_json) {
        api_key_ninjas = config_json["api_key_ninjas"].get<std::string>();
        city_list = config_json["city"].get<std::vector<std::string>>();
        days = config_json["days"].get<long long>();
        reload_freq_sec = config_json["reload_freq_sec"].get<size_t>();
    }
};

struct CoordResponse {
    int status_code = 0;

    double longitude = 0;
    double latitude = 0;

    bool error = false;
    std::string error_reason;

    CoordResponse() = default;

    explicit CoordResponse(const cpr::Response& response) {
        status_code = response.status_code;
        if (status_code == 200) {
            json city = json::parse(response.text);
            if (!city.empty()) {
                longitude = city[0]["longitude"].get<double>();
                latitude = city[0]["latitude"].get<double>();
            } else {
                error = true;
                error_reason = "Wrong city name! Go to the next or previous city.";
            }
        } else {
            error = true;
            error_reason = "No Internet connection!";
        }
    }
};

struct WeatherResponse {
    int status_code;

    double current_temperature;
    int current_weather_code;
    double current_wind_speed;
    int current_wind_direction;

    std::vector<std::string> time_stamp;
    std::vector<double> temperature;
    std::vector<int> weather_code;
    std::vector<double> wind_speed;
    std::vector<int> wind_direction;
    std::vector<double> precipitation;
    std::vector<int> precipitation_probability;

    bool error = false;
    std::string error_reason;

    WeatherResponse() = default;

    explicit WeatherResponse(const cpr::Response& response) {
        status_code = response.status_code;
        if (status_code != 0) {
            json weather = json::parse(response.text);

            if (!weather.contains("error")) {
                current_temperature = weather["current_weather"]["temperature"].get<double>();
                current_weather_code = weather["current_weather"]["weathercode"].get<int>();
                current_wind_speed = weather["current_weather"]["windspeed"].get<double>();
                current_wind_direction = weather["current_weather"]["winddirection"].get<int>();

                time_stamp = weather["hourly"]["time"].get<std::vector<std::string>>();


                for (auto& elem: weather["hourly"]["temperature_2m"]) {
                    temperature.emplace_back(elem.is_null() ? INT_MIN : elem.get<double>());
                }
                for (auto& elem: weather["hourly"]["weathercode"]) {
                    weather_code.emplace_back(elem.is_null() ? -1 : elem.get<int>());
                }
                for (auto& elem: weather["hourly"]["windspeed_10m"]) {
                    wind_speed.emplace_back(elem.is_null() ? INT_MIN : elem.get<double>());
                }
                for (auto& elem: weather["hourly"]["winddirection_10m"]) {
                    wind_direction.emplace_back(elem.is_null() ? INT_MIN : elem.get<int>());
                }
                for (auto& elem: weather["hourly"]["precipitation"]) {
                    precipitation.emplace_back(elem.is_null() ? 0 : elem.get<double>());
                }
                for (auto& elem: weather["hourly"]["precipitation_probability"]) {
                    precipitation_probability.emplace_back(elem.is_null() ? INT_MIN : elem.get<int>());
                }

            } else {
                error = true;
                error_reason = weather["reason"].get<std::string>();
            }
        } else {
            error = true;
            error_reason = "No Internet connection!";
        }
    }
};

#endif //LABWORK10_STRUCTURES_H
