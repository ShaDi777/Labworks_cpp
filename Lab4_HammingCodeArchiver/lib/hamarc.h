#pragma once

#include <cstdio>
#include <bitset>
#include <filesystem>
#include <cmath>
#include <iostream>

#include "options.h"


class HamArc {
private:
    std::string path;
    std::vector<std::string> files;
    int kParityBits;
    int kTotalBits;
    int kDataBits;

    static void SetParityBits(std::vector<bool>& block, int kParityBits, int kTotalBits);

    static void ValidateBlock(std::vector<bool>& block, int kParityBits, int kTotalBits);

    static std::string GetFileNameFromPath(const char* file_path);

    static uint64_t GetOffsetSize(FILE* archive);

    void WriteEncoded(int mode, std::string file_name, FILE* input, FILE* archive);

    std::string DecodeFileName(FILE* archive);

    // Current position must be in the beginning of FILE_DATA
    void DecodeFile(FILE* archive, const std::string& FILE_NAME, uint64_t total_offset);

public:
    HamArc(std::string path, std::vector<std::string> files, int parityBitsCount) {
        this->path = std::move(path);
        this->files = std::move(files);
        this->kParityBits = parityBitsCount;
        this->kTotalBits = (1 << parityBitsCount);
        this->kDataBits = kTotalBits - kParityBits - 1;
    }

    void ClearArchiveFile();

    void Encode(const char* src);

    void Decode(const char* file_name);

    void DeleteFiles();

    void Concatenate();

    void List();
};
