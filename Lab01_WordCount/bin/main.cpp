#include "lib/WordCount.h"

int main(int argc, char *argv[]) {
    // Reading ARGS
    FileInfo file_info;
    std::vector<std::string> optinons;
    std::vector<std::string> files;
    ReadArgs(argc, argv, optinons, files);

    // Checking options and files
    SortOptionsFiles(file_info, optinons, files);

    //Opening files and creating output
    if (files.empty()) {
        std::cout << "No files to open";
        return 1;
    }
    for (const std::string &filename: files) {
        PrintInfoFile(filename, file_info);
    }

    return 0;
}