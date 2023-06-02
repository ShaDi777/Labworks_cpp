#pragma once

#include "WordCount.h"

void ReadArgs(int argc, char *argv[],
              std::vector<std::string> &optinons, std::vector<std::string> &files) {
    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        if (argv[i][0] == '-') {
            optinons.push_back(arg);
        } else {
            files.push_back(arg);
        }
    }
}

void SortOptionsFiles(FileInfo &file_info,
                      std::vector<std::string> &optinons,
                      std::vector<std::string> &files) {
    if (optinons.empty()) {
        file_info.print_lines = true;
        file_info.print_words = true;
        file_info.print_bytes = true;
        file_info.print_chars = true;
    } else {
        for (std::string &optinon: optinons) {
            //Checking options with --
            if (optinon[0] == optinon[1] && optinon[1] == '-') {
                switch (optinon[2]) {
                    case 'l':
                        file_info.print_lines = true;
                        break;
                    case 'w':
                        file_info.print_words = true;
                        break;
                    case 'b':
                        file_info.print_bytes = true;
                        break;
                    case 'c':
                        file_info.print_chars = true;
                        break;
                    default:
                        std::cout << "Wrong --option" << std::endl;
                }
            } else {
                //Checking options with -
                std::vector<char> options_stack;
                for (char c: optinon) {
                    switch (c) {
                        case '-':
                            options_stack.push_back('-');
                            break;
                        case 'l':
                            options_stack.push_back('l');
                            break;
                        case 'w':
                            options_stack.push_back('w');
                            break;
                        case 'c':
                            options_stack.push_back('c');
                            break;
                        case 'm':
                            options_stack.push_back('m');
                            break;
                        default:
                            std::cout << "Wrong -option" << std::endl;
                            options_stack.clear();
                            break;
                    }
                    if (options_stack.empty()) {
                        break;
                    }
                }
                for (char opt: options_stack) {
                    switch (opt) {
                        case '-':
                            break;
                        case 'l':
                            file_info.print_lines = true;
                            break;
                        case 'w':
                            file_info.print_words = true;
                            break;
                        case 'c':
                            file_info.print_bytes = true;
                            break;
                        case 'm':
                            file_info.print_chars = true;
                            break;
                        default:
                            break;
                    }
                }
            }
        }
    }
}

void ResetFile(std::ifstream &file) {
    file.clear();
    file.seekg(0, std::ios::beg);
}

uint64_t FileLinesCount(std::ifstream &file) {
    ResetFile(file);
    uint64_t count_lines = 1;
    while (file.good()) {
        if (file.get() == '\n') {
            count_lines++;
        }
    }

    return count_lines;
}

uint64_t FileSizeBytes(std::ifstream &file, const std::string &filename) {
    ResetFile(file);
    return std::filesystem::file_size(filename);
}

uint64_t FileWordCount(std::ifstream &file) {
    ResetFile(file);
    uint64_t word_count = 0;
    std::string buffer;
    while (file >> buffer) {
        word_count++;
    }
    return word_count;
}

uint64_t FileCharCount(std::ifstream &file) {
    ResetFile(file);
    uint64_t char_count = 0;
    char buffer;
    while (file >> std::noskipws >> buffer) {
        if (isprint(buffer)) {
            char_count++;
        }
    }
    return char_count;
}

//Used for multi-language input
std::wstring widen(const std::string &utf8_string) {
    std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> convert;
    return convert.from_bytes(utf8_string);
}

uint64_t FileWideCharCount(std::ifstream &file) {
    ResetFile(file);
    uint64_t char_count = 0;
    std::string buffer;
    while (file >> buffer) {
        std::wstring temp = widen(buffer);
        char_count += temp.size();
    }
    return char_count;
}

void PrintInfoFile(const std::string& filename, FileInfo& file_info) {
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        std::cout << "Error opening file: " << filename << std::endl;
        return;
    }
    //Output
    if (file_info.print_lines) {
        std::cout << FileLinesCount(file) << " ";
    }
    if (file_info.print_words) {
        std::cout << FileWordCount(file) << " ";
    }
    if (file_info.print_bytes) {
        std::cout << FileSizeBytes(file, filename) << " ";
    }
    if (file_info.print_chars) {
        std::cout << FileCharCount(file) << " ";
    }
    std::cout << filename << std::endl;
}
