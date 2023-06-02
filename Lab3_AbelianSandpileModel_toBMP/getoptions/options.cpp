#pragma once

#include "options.h"

void CheckLongOption (uint16_t& iter, char** argv, Info& info) {
    const char* arg = argv[iter];
    if (strcmp(arg, "--length") == 0) {
        info.height = atol(argv[++iter]);
    } else if (strcmp(arg, "--width") == 0) {
        info.width = atol(argv[++iter]);
    } else if (strcmp(arg, "--input") == 0) {
        info.input_dir = argv[++iter];
    } else if (strcmp(arg, "--output") == 0) {
        info.output_dir = argv[++iter];
    } else if (strcmp(arg, "--max-iter") == 0) {
        info.max_iter = atoll(argv[++iter]);
    } else if (strcmp(arg, "-- freq") == 0) {
        info.freq = atoll(argv[++iter]);
    }
}

void CheckOption(uint16_t& iter, char** argv, Info& info) {
    const char* arg = argv[iter];
    if (arg[0]!='-'){
        return;
    }
    switch (arg[1]) {
        case '-':
            CheckLongOption(iter, argv, info);
            break;
        case 'l':
            info.height = atol(argv[++iter]);
            break;
        case 'w':
            info.width = atol(argv[++iter]);
            break;
        case 'i':
            info.input_dir = argv[++iter];
            break;
        case 'o':
            info.output_dir = argv[++iter];
            break;
        case 'm':
            info.max_iter = atoll(argv[++iter]);
            break;
        case 'f':
            info.freq = atoll(argv[++iter]);
            break;
        default:
            break;
    }
}

void ReadArgs(int argc, char** argv, Info& info) {
    uint16_t iter = 0;
    while (iter < argc) {
        CheckOption(iter, argv, info);
        iter++;
    }
}
