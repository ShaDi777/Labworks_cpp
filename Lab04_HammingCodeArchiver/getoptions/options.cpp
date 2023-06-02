#include "options.h"

void CheckLongOption(uint16_t& iter, char** argv, Info& info) {
    char* arg = argv[iter];
    if (strcmp(arg, "--create") == 0) {
        info.mode = MODE::create;
    } else if (strcmp(arg, "--list") == 0) {
        info.mode = MODE::list;
    } else if (strcmp(arg, "--extract") == 0) {
        info.mode = MODE::extract;
    } else if (strcmp(arg, "--append") == 0) {
        info.mode = MODE::append;
    } else if (strcmp(arg, "--delete") == 0) {
        info.mode = MODE::rmv;
    } else if (strcmp(arg, "--concatenate") == 0) {
        info.mode = MODE::concat;
    } else {
        char* result = strstr(arg, "--file=");
        if (result != nullptr) {
            info.archive_name = result + 7;
        }
    }
}

void CheckOption(uint16_t& iter, int argc, char** argv, Info& info) {
    char* arg = argv[iter];
    if (arg[0] != '-') {
        info.file_names.emplace_back(arg);
        return;
    }
    switch (arg[1]) {
        case '-':
            CheckLongOption(iter, argv, info);
            break;
        case 'c':
            info.mode = MODE::create;
            break;
        case 'l':
            info.mode = MODE::list;
            break;
        case 'x':
            info.mode = MODE::extract;
            break;
        case 'a':
            info.mode = MODE::append;
            break;
        case 'd':
            info.mode = MODE::rmv;
            break;
        case 'A':
            info.mode = MODE::concat;
            break;
        case 'f':
            if (iter + 1 < argc)
                info.archive_name = argv[++iter];
            else
                info.archive_name = "new_archive.haf";
            break;
        case 'p':
            if (iter + 1 < argc)
                info.parityBitCount = atoi(argv[++iter]);
            if (info.parityBitCount <= 2 || info.parityBitCount > 10)
                info.parityBitCount = 3;
            break;
        default:
            break;
    }
}

void ReadArgs(int argc, char** argv, Info& info) {
    uint16_t iter = 1;
    while (iter < argc) {
        CheckOption(iter, argc, argv, info);
        iter++;
    }
}
