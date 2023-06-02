#include <iostream>
#include <fstream>

#include "getoptions/options.h"
#include "lib/bmp.h"
#include "lib/table.h"

char* PathToSave(uint64_t& iter, Info& info) {
    char* path = new char[1024];
    strcpy(path, info.output_dir);
    strcat(path, "\\\\");
    char name[25];
    itoa(iter, name, 10);
    strcat(name, ".bmp");
    strcat(path, name);
    return path;
}

int main(int argc, char** argv) {
    double start_time = clock();

    Info info;
    ReadArgs(argc, argv, info);

    uint64_t* table = new uint64_t[info.height * info.width];
    for (size_t i = 0; i < info.width * info.height; i++) {
        table[i] = 0;
    }

    std::ifstream in(info.input_dir, std::ios::binary);
    uint16_t x = 0;
    uint16_t y = 0;
    uint64_t value;
    if (!in.is_open()) {
        std::cout << "InputFile didn't open!";
        return 1;
    }
    while (in.good()) {
        in >> x;
        in >> y;
        in >> value;
        table[y * info.width + x] = value;
    }

    Borders borders;
    CountPadding(table, info.height, info.width, borders);

    uint64_t iter = 1;
    while (!IterateTable(table, info.height, info.width, borders) && iter <= info.max_iter) {
        if (info.freq != 0 && iter % info.freq == 0) {
            char* path = PathToSave(iter, info);

            int temp_padding[] = {0, 0, 0, 0};
            for (uint8_t side = UP; side < kNumSides; side++) {
                temp_padding[side] = (!borders.upscaled[side] ? 0 : borders.padding[side]);
            }

            CreateBMP(path, table, info.height, info.width, temp_padding);
            delete[] path;
        }
        iter++;
    }

    for (uint8_t side = UP; side < kNumSides; side++) {
        borders.padding[side] = (!borders.upscaled[side] ? 0 : borders.padding[side]);
    }

    char* path = info.output_dir;
    strcat(path, "\\\\FINAL-BIG.bmp");
    CreateBMP(path, table, info.height, info.width, borders.padding);

    delete[] table;

    double end_time = clock();
    std::cout << (end_time - start_time) / CLOCKS_PER_SEC;
    return 0;
}

// 100k -- 2,5 sec
// 200k -- 9,5 sec
// 300k -- 21 sec
// 400k -- 37 sec
// 500k -- 58 sec
// 1000k - 230 sec