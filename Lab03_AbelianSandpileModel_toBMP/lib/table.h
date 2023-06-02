#pragma once

#include <iostream>

enum SIDE {
    UP = 0, DOWN, LEFT, RIGHT
};
const int kNumSides = 4;

struct Borders {
    int32_t padding[4] = {0, 0, 0, 0};
    int8_t resize[4] = {0, 0, 0, 0};
    bool upscaled[4] = {false, false, false, false};
};

bool IterateTable(uint64_t*& table, uint16_t& height, uint16_t& width, Borders& borders);

void CountPadding(uint64_t* table, uint16_t height, uint16_t width, Borders& borders);

void PrintTable(uint64_t* table, uint16_t height, uint16_t width);

void PrintTableShortened(uint64_t* table, uint16_t height, uint16_t width);
