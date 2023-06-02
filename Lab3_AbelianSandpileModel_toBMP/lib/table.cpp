#include "table.h"

void Realloc(uint64_t*& table, uint16_t& height, uint16_t& width, Borders& borders) {
    uint16_t new_width = (width + borders.resize[LEFT] * (borders.padding[LEFT] + 1) +
                          borders.resize[RIGHT] * (borders.padding[RIGHT] + 1));
    uint16_t new_height = (height + borders.resize[UP] * (borders.padding[UP] + 1) +
                           borders.resize[DOWN] * (borders.padding[DOWN] + 1));
    uint64_t* new_table = new uint64_t[new_height * new_width]();

    for (size_t i = 0; i < height; i++) {
        for (size_t j = 0; j < width; j++) {
            new_table[(i + borders.resize[UP] * (borders.padding[UP] + 1)) * new_width + j +
                      borders.resize[LEFT] * (borders.padding[LEFT] + 1)] = table[i * width + j];
        }
    }
    width = new_width;
    height = new_height;

    delete[] table;
    table = new_table;
}

bool IterateTable(uint64_t*& table, uint16_t& height, uint16_t& width, Borders& borders) {

    for (size_t j = borders.padding[LEFT]; j < (width - borders.padding[RIGHT]); j++) {
        if (table[borders.padding[UP] * width + j] >= 4) {
            borders.resize[UP]--;
            break;
        }
    }


    for (size_t j = borders.padding[LEFT]; j < (width - borders.padding[RIGHT]); j++) {
        if (table[(height - borders.padding[DOWN] - 1) * width + j] >= 4) {
            borders.resize[DOWN]--;
            break;
        }
    }


    for (size_t i = borders.padding[UP]; i < (height - borders.padding[DOWN]); i++) {
        if (table[i * width + width - borders.padding[RIGHT] - 1] >= 4) {
            borders.resize[RIGHT]--;
            break;
        }
    }


    for (size_t i = borders.padding[UP]; i < (height - borders.padding[DOWN]); i++) {
        if (table[i * width + borders.padding[LEFT]] >= 4) {
            borders.resize[LEFT]--;
            break;
        }
    }


    for (int side = UP; side < kNumSides; side++) {
        borders.padding[side] += borders.resize[side];
        borders.resize[side] = 0;
    }


    bool realloc = ((borders.padding[UP] < 0) ||
                    (borders.padding[DOWN] < 0) ||
                    (borders.padding[LEFT] < 0) ||
                    (borders.padding[RIGHT] < 0));

    if (realloc) {
        for (int side = UP; side < kNumSides; side++) {
            if (borders.padding[side] < 0) {
                if (side == UP || side == DOWN) {
                    borders.padding[side] = height / 2;
                } else {
                    borders.padding[side] = width / 2;
                }

                borders.resize[side] = 1;
                borders.upscaled[side] = true;
            }
        }

        Realloc(table, height, width, borders);

        for (int side = UP; side < kNumSides; side++) {
            borders.resize[side] = 0;
        }

    }

    bool is_stable = true;
    for (size_t i = borders.padding[UP] + 1; i < (height - borders.padding[DOWN] - 1); i++) {
        for (size_t j = borders.padding[LEFT] + 1; j < (width - borders.padding[RIGHT] - 1); j++) {
            if (table[i * width + j] >= 4) {
                table[(i - 1) * width + j] += 1;
                table[(i + 1) * width + j] += 1;
                table[i * width + j + 1] += 1;
                table[i * width + j - 1] += 1;
                table[i * width + j] -= 4;
                is_stable = false;
            }
        }
    }


    return is_stable;
}

void CountPadding(uint64_t* table, const uint16_t height, const uint16_t width, Borders& borders) {
    bool zeros = true;
    for (size_t i = 0; i < height; i++) {
        for (size_t j = 0; j < width; j++) {
            if (table[i * width + j] != 0) {
                zeros = false;
                break;
            }
        }
        if (zeros) borders.padding[UP]++;
        else break;
    }

    zeros = true;
    for (size_t j = 0; j < width; j++) {
        for (size_t i = 0; i < height; i++) {
            if (table[i * width + j] != 0) {
                zeros = false;
                break;
            }
        }
        if (zeros) borders.padding[LEFT]++;
        else break;
    }

    zeros = true;
    for (int j = width - 1; j >= 0; j--) {
        for (size_t i = 0; i < height; i++) {
            if (table[i * width + j] != 0) {
                zeros = false;
                break;
            }
        }
        if (zeros) borders.padding[RIGHT]++;
        else break;
    }

    zeros = true;
    for (int i = height - 1; i >= 0; i--) {
        for (size_t j = 0; j < width; j++) {
            if (table[i * width + j] != 0) {
                zeros = false;
                break;
            }
        }
        if (zeros) borders.padding[DOWN]++;
        else break;
    }
}

void PrintTable(uint64_t* table, const uint16_t height, const uint16_t width) {
    for (size_t i = 0; i < height; i++) {
        for (size_t j = 0; j < width; j++) {
            std::cout << table[i * width + j] << ' ';
        }
        std::cout << std::endl;
    }
}

void PrintTableShortened(uint64_t* table, const uint16_t height, const uint16_t width, Borders& borders) {
    for (size_t i = borders.padding[UP]; i < height - borders.padding[DOWN]; i++) {
        for (size_t j = borders.padding[LEFT]; j < width - borders.padding[RIGHT]; j++) {
            std::cout << table[i * width + j] << ' ';
        }
        std::cout << std::endl;
    }
}
