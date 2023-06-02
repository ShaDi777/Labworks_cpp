#include "getoptions/options.h"
#include "lib/hamarc.h"


int main(int argc, char** argv) {
    Info info;
    ReadArgs(argc, argv, info);
    HamArc my_archive = HamArc(info.archive_name, info.file_names, info.parityBitCount);

    switch (info.mode) {
        case create:
            my_archive.ClearArchiveFile();
            for (const auto& file_name: info.file_names) {
                my_archive.Encode(file_name.c_str());
            }
            break;
        case extract:
            if (info.file_names.empty()) {
                my_archive.Decode("");
            } else {
                for (const auto& file_name: info.file_names) {
                    my_archive.Decode(file_name.c_str());
                }
            }
            break;
        case append:
            for (const auto& file_name: info.file_names) {
                my_archive.Encode(file_name.c_str());
            }
            break;
        case concat:
            my_archive.Concatenate();
            break;
        case rmv:
            my_archive.DeleteFiles();
            break;
        case list:
            my_archive.List();
            break;
        default:
            break;
    }

    return 0;
}
