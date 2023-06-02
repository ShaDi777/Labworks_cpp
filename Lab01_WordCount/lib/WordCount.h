#include <iostream>

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <filesystem>
#include <codecvt>

struct FileInfo {
    bool print_lines = false;
    bool print_words = false;
    bool print_bytes = false;
    bool print_chars = false;
};

void ReadArgs(int argc, char* argv[],
              std::vector<std::string>& optinons, std::vector<std::string>& files);

void SortOptionsFiles(FileInfo& file_info,
                      std::vector<std::string>& optinons,
                      std::vector<std::string>& files);

void ResetFile(std::ifstream& file);

uint64_t FileLinesCount(std::ifstream& file);

uint64_t FileSizeBytes(std::ifstream& file, const std::string& filename);

uint64_t FileWordCount(std::ifstream& file);

uint64_t FileCharCount(std::ifstream& file);

void PrintInfoFile(const std::string& filename, FileInfo& file_info);