#include "structures.h"
#include "constants.h"

std::string Direction(int degree) {
    if (degree < 0) {
        return "Unknown direction";
    } else if (degree < 22.5 || degree > 337.5) {
        return "North";
    } else if (22.5 < degree && degree < 67.5) {
        return "North-East";
    } else if (67.5 < degree && degree < 112.5) {
        return "East";
    } else if (112.5 < degree && degree < 157.5) {
        return "South-East";
    } else if (157.5 < degree && degree < 202.5) {
        return "South";
    } else if (202.5 < degree && degree < 247.5) {
        return "South-West";
    } else if (247.5 < degree && degree < 292.5) {
        return "West";
    } else if (292.5 < degree && degree < 337.5) {
        return "North-West";
    }
    return "Unknown";
}

std::string ColorValue(int value) {
    std::string result = "\033[38;5;";
    if (value <= -1000) {
        result.append("255m?\033[0m");
        return result;
    } else if (value <= -20) {
        result.append("111m");
    } else if (value <= -15) {
        result.append("105m");
    } else if (value <= -10) {
        result.append("117m");
    } else if (value <= -5) {
        result.append("123m");
    } else if (value <= 0) {
        result.append("159m");
    } else if (value <= 5) {
        result.append("120m");
    } else if (value <= 10) {
        result.append("118m");
    } else if (value <= 15) {
        result.append("227m");
    } else if (value <= 20) {
        result.append("214m");
    } else if (value <= 25) {
        result.append("208m");
    } else {
        result.append("196m");
    }
    result.append(std::to_string(value));
    result.append("\033[0m");
    return result;
}

int dayofweek(int d, int m, int y) {
    static int t[] = {0, 3, 2, 5, 0, 3,
                      5, 1, 4, 6, 2, 4};
    y -= m < 3;
    return (y + y / 4 - y / 100 +
            y / 400 + t[m - 1] + d) % 7;
}

void TitleCell(std::vector<std::string>& table, const std::string& time_stamp, const std::string& head_line) {
    int year = stoi(time_stamp.substr(0, 4));
    int month = stoi(time_stamp.substr(5, 2));
    int day = stoi(time_stamp.substr(8, 2));

    std::string date(m_dayofweek[dayofweek(day, month, year)]);
    date.append(" ");
    date.append(std::to_string(day) + " ");
    date.append(m_month[month - 1]);

    table.emplace_back(head_line);
    int delta = (head_line.length() - 2 - date.length());
    int padding_left = delta / 2;
    int padding_right = (delta / 2) + (delta % 2);
    std::string header = "|";
    while (padding_left--) {
        header.append(" ");
    }
    header.append(date);
    while (padding_right--) {
        header.append(" ");
    }
    header.append("|");

    table.emplace_back(header);
    table.emplace_back(head_line);
}

std::vector<std::string> WeatherCell(const std::vector<double>& code) {
    int info_size = 30 - 13;
    std::vector<std::string> cell;
    std::vector<std::string> current_weather;
    if (WEATHER_PICTURE_NAME.find((int) code[0]) != WEATHER_PICTURE_NAME.end() &&
        WEATHER_SYMBOLS.find(WEATHER_PICTURE_NAME.at((int) code[0])) != WEATHER_SYMBOLS.end()) {
        current_weather = WEATHER_SYMBOLS.at(WEATHER_PICTURE_NAME.at((int) code[0]));
    } else {
        current_weather = WEATHER_SYMBOLS.at("Unknown");
    }

    std::string temporary;
    if (WEATHER_NAME.find((int) code[0]) != WEATHER_NAME.end()) {
        temporary = WEATHER_NAME.at((int) code[0]);
    } else {
        temporary = "Unknown";
    }
    temporary.resize(info_size, ' ');
    cell.emplace_back(current_weather[0] + temporary);

    temporary.clear();
    temporary = ((int) code[1] > 0 ? "+" : "") + ColorValue((int) round(code[1])) + " °C";
    temporary.resize(info_size + 16, ' ');
    cell.emplace_back(current_weather[1] + temporary);

    temporary.clear();
    temporary = ColorValue((int) round(code[2])) + " km/h";
    temporary.resize(info_size + 15, ' ');
    cell.emplace_back(current_weather[2] + temporary);

    temporary.clear();
    temporary = Direction((int) code[3]);
    temporary.resize(info_size, ' ');
    cell.emplace_back(current_weather[3] + temporary);

    temporary.clear();
    temporary = std::to_string(code[4]);
    temporary = temporary.substr(0, temporary.find('.') + 2) + " mm | ";
    temporary += std::to_string((int) code[5]) + "%";
    temporary.resize(info_size, ' ');
    cell.emplace_back(current_weather[4] + temporary);

    return cell;
}

void WeatherLine(std::vector<std::string>& table,
                 const std::vector<double>& morning_code,
                 const std::vector<double>& afternoon_code,
                 const std::vector<double>& evening_code,
                 const std::vector<double>& night_code) {
    std::vector<std::string> morning = WeatherCell(morning_code);
    std::vector<std::string> afternoon = WeatherCell(afternoon_code);
    std::vector<std::string> evening = WeatherCell(evening_code);
    std::vector<std::string> night = WeatherCell(night_code);

    for (int i = 0; i < 5; i++) {
        std::string line = "|" + morning[i] + "|" +
                           afternoon[i] + "|" +
                           evening[i] + "|" +
                           night[i] + "|";

        table.emplace_back(line);
    }
}


void CreateTable(WeatherResponse& weather) {
    std::vector<std::string> table;
    std::string strip_line =   "+------------------------------+------------------------------+------------------------------+------------------------------+";
    std::string daytime_line = "|            Morning           |           Afternoon          |            Evening           |             Night            |";
    std::string head_line =    "+---------------------------------------------------------------------------------------------------------------------------+";

    std::vector<double> current = {(double) weather.current_weather_code,
                                   weather.current_temperature,
                                   weather.current_wind_speed,
                                   (double) weather.current_wind_direction,
                                   0};
    for (auto& x: WeatherCell(current)) {
        table.emplace_back(x);
    }
    table.emplace_back("\n");

    for (size_t i = 0; i < weather.time_stamp.size(); i += 24) {
        TitleCell(table, weather.time_stamp[i], head_line);
        table.emplace_back(daytime_line);
        table.emplace_back(strip_line);

        std::vector<std::vector<double>> day_codes = {
                {0, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0, 0}
        };
        for (int j = 0; j < 6; j++) {
            for (int k = 0; k < 4; k++) {
                day_codes[k][0] = weather.weather_code[i + 3 + 6 * k];
                day_codes[k][1] += weather.temperature[i + j + 6 * k] / 6;
                day_codes[k][2] += weather.wind_speed[i + j + 6 * k] / 6;
                day_codes[k][3] += (double) weather.wind_direction[i + j + 6 * k] / 6;
                day_codes[k][4] += weather.precipitation[i + j + 6 * k] / 6;
                day_codes[k][5] = std::max(day_codes[k][5], (double) weather.precipitation_probability[i + j + 6 * k]);
            }
        }

        WeatherLine(table, day_codes[0], day_codes[1], day_codes[2], day_codes[3]);

        table.emplace_back(head_line);
        table.emplace_back("\n\n");
    }

    for (auto& line: table) {
        std::cout << line << "\n";
    }
}
