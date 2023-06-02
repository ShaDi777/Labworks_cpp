#pragma once

#include <iostream>
#include <cstring>
#include <vector>

enum MODE {
    create = 0, list, extract, append, rmv, concat
};

struct Info {
    MODE mode;
    char* archive_name;
    std::vector<std::string> file_names;
    uint16_t parityBitCount;
};

void CheckLongOption(uint16_t& iter, char** argv, Info& info);

void CheckOption(uint16_t& iter, char** argv, Info& info);

void ReadArgs(int argc, char** argv, Info& info);
