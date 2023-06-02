#pragma once

#include <cstdint>
#include <cstdio>


const int BITS_PER_PIXEL = 4;
const int COLOR_COUNT = 5;
const int FILE_HEADER_SIZE = 14;
const int INFO_HEADER_SIZE = 40;

struct Color {
    unsigned char blue;
    unsigned char green;
    unsigned char red;
    unsigned char reserved = 0;
};

const Color WHITE = {255, 255, 255};
const Color GREEN = {0, 187, 0};
const Color PURPLE = {187, 0, 187};
const Color YELLOW = {0, 255, 255};
const Color BLACK = {0, 0, 0};

const Color COLOR_TABLE[COLOR_COUNT] = {WHITE, GREEN, PURPLE, YELLOW, BLACK};

enum ColorPick {
    white = 0, green, purple, yellow, black
};

void CreateBMP(const char* file_name, const uint64_t* image, uint16_t height, uint16_t width, const int* indent);
