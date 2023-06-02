#pragma once

#include <iostream>
#include <cstring>

struct Info {
    uint16_t height = 0;
    uint16_t width= 0;
    char* input_dir;
    char* output_dir;
    uint64_t max_iter = 0;
    uint64_t freq = 0;
};

void CheckLongOption (uint16_t& iter, char** argv, Info& info);

void CheckOption(uint16_t& iter, char** argv, Info& info);

void ReadArgs(int argc, char** argv, Info& info);
