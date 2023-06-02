#include <iostream>
#include <fstream>

#include "ID3v2/ID3v2Parser.h"
#include "ArgParser.h"

int main(int argc, char** argv) {
    ArgumentParser::ArgParser parser("MP3_parser");
    parser.AddStringArgument('p', "path", "Path to mp3 file");
    parser.AddStringArgument('g', "get", "Get frames with passed name").Default("");
    if (!parser.Parse(argc, argv)) {
        std::cerr << "Wrong arguments\n";
        return 1;
    }

    std::string file_path = parser.GetStringValue("path");
    std::string frame_id = parser.GetStringValue("get");

    ID3v2 data = ParseMP3(file_path);

    if (frame_id.empty()) {
        for (auto& frame: data.frames) {
            frame->printInfo();
        }
    } else {
        std::vector<Frame*> frame_ptrs = GetFrame(data, frame_id.c_str());
        for (auto fp: frame_ptrs) {
            fp->printInfo();
        }
    }

    return 0;
}
