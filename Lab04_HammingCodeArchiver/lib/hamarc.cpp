#include "hamarc.h"

void printBlock(const std::vector<bool>& block) {
    for (auto b : block) {
        std::cout << b << ' ';
    }
    std::cout << '\n';
}

const char separator = 0;

void HamArc::ClearArchiveFile() {
    FILE* temp = fopen(this->path.c_str(), "wb");
    fclose(temp);
}

void HamArc::SetParityBits(std::vector<bool>& block, int kParityBits, int kTotalBits) {
    // Set parity bits
    for (int i = 0; i < kParityBits; i++) {
        int bit = (1 << i);
        for (int begin = bit, end = begin + bit;
             begin < kTotalBits;
             begin += (1 << i) * 2, end = begin + bit) {
            if (end > kTotalBits - 1) end = kTotalBits;
            for (int j = begin; j < end; ++j) {
                if (block[j]) block[bit].flip();
            }
        }
    }

    //Set extended parity bit
    for (int i = 1; i < kTotalBits; i++) {
        if (block[i]) block[0].flip();
    }
}

void HamArc::ValidateBlock(std::vector<bool>& block, int kParityBits, int kTotalBits) {
    bool extended_error = false;
    for (int i = 0; i < kTotalBits; i++) {
        extended_error ^= block[i];
    }

    int syndrome = 0;
    for (int i = 0; i < kParityBits; i++) {
        bool s = false;
        int bit = (1 << i);
        for (int begin = bit, end = begin + bit;
             begin < kTotalBits;
             begin += (1 << i) * 2, end = begin + bit) {
            if (end > kTotalBits - 1) end = kTotalBits;
            for (int j = begin; j < end; ++j) {
                s ^= block[j];
            }
        }
        syndrome += (s << i);
    }
    if (extended_error && syndrome > 0) {
        block[syndrome].flip();
    } else if (!extended_error && syndrome > 0) {
        std::cerr << "Archive has double bit error!\n";
        exit(EXIT_FAILURE);
    }
}

std::string HamArc::GetFileNameFromPath(const char* file_path) {
    std::string s = file_path;
    uint64_t pos = s.find_last_of('\\');
    return s.substr(++pos, s.length());
}

uint64_t HamArc::GetOffsetSize(FILE* archive) {
    const int parityBits = 6;
    const int totalBits = 64;
    std::vector<bool> offset(totalBits);
    int ch = 0;

    for (int i = 0; i < totalBits; i++) {
        if (i % 8 == 0) {
            ch = getc(archive);
        }
        offset[i] = ((ch >> (i % 8)) & 1);
    }

    ValidateBlock(offset, parityBits, totalBits);

    uint64_t size = 0;
    int pbits = 0;
    for (int i = 1; i < totalBits; i++) {
        if (i == (1 << pbits)) {
            pbits++;
            continue;
        }
        size += (offset[i] << (i - pbits - 1));
    }

    return size;
}

void HamArc::WriteEncoded(int mode, std::string file_name, FILE* input, FILE* archive) {
    // Mode 0 - Name encoding
    // Mode 1 - Data encoding
    std::vector<bool> block(kTotalBits);
    int parity_bit = 0;
    int data_bit = 0;
    int name_iter = 0;
    int ch;
    if (mode == 0) ch = file_name[name_iter++];
    else if (mode == 1) ch = getc(input);

    unsigned char out_byte = 0;
    int out_byte_iter = 0;

    // Encoding File (Name/Data)
    while (true) {
        // Write single Block
        parity_bit = 0;
        block[0] = false;
        for (int i = 1; i < kTotalBits; i++) {
            if (i == (1 << parity_bit)) {
                block[i] = false;
                parity_bit++;
                continue;
            }
            block[i] = ((ch >> data_bit) & 1);
            data_bit++;
            if (data_bit == 8) {
                data_bit = 0;
                if (mode == 0) {
                    if (file_name.length() == name_iter) {
                        ch = 0;
                        break;
                    }
                    ch = file_name[name_iter++];
                } else if (mode == 1) {
                    ch = getc(input);
                    if (ch == EOF) {
                        break;
                    }
                }
            }
        }

        SetParityBits(block, kParityBits, kTotalBits);

        // Block into byte
        for (int i = 0; i < kTotalBits; i++) {
            out_byte += (block[i] << out_byte_iter);
            out_byte_iter++;
            if (out_byte_iter == 8) {
                out_byte_iter = 0;
                fwrite(&out_byte, 1, 1, archive);
                out_byte = 0;
            }
        }

        std::fill(block.begin(), block.end(), 0);

        if ((mode == 0 && file_name.length() == name_iter && ch == 0)
            || (mode == 1 && ch == EOF)) {
            if (out_byte_iter > 0) {
                for (int i = 0; i < kTotalBits; i++) {
                    out_byte += (block[i] << out_byte_iter);
                    out_byte_iter++;
                    if (out_byte_iter == 8) {
                        out_byte_iter = 0;
                        fwrite(&out_byte, 1, 1, archive);
                        out_byte = 0;
                    }
                }
                //out_byte_iter = 0;
                //fwrite(&out_byte, 1, 1, archive);
                //out_byte = 0;
            }
            break;
        }
    }
}

void HamArc::Encode(const char* src) {

    FILE* archive = fopen(this->path.c_str(), "ab+");

    FILE* file = fopen(src, "rb");
    if (file == nullptr) {
        return;
    }

    // Name
    std::string file_name = GetFileNameFromPath(src);
    file_name += separator;
    uint64_t file_name_size = (file_name.length()) * 8;
    file_name_size = (file_name_size / kDataBits + ((file_name_size % kDataBits) > 0)) * kTotalBits;
    file_name_size = ceil((double) file_name_size / 8);


    // Size
    uint64_t file_size = std::filesystem::file_size(src) * 8; // File size in bits
    file_size = (file_size / kDataBits + ((file_size % kDataBits) > 0)) * kTotalBits; // encoded file size in bits
    file_size = ceil((double) file_size / 8); // encoded file size in bytes


    // Offset
    uint64_t encoded_size = sizeof(file_size) + file_name_size + file_size;

    const int parityBits = 6;
    const int totalBits = 64;
    std::vector<bool> block(totalBits);

    int pbit = 0;
    int stored_bit = 0;
    for (int i = 1; i < totalBits; i++) {
        if (i == (1 << pbit)) {
            pbit++;
            continue;
        }
        block[i] = ((encoded_size >> stored_bit++) & 1);
    }

    SetParityBits(block, parityBits, totalBits);

    for (int i = 0; i < totalBits; i += 8) {
        unsigned char c = 0;
        for (int j = i; j < (i + 8); j++) {
            c += (block[j] << (j % 8));
        }
        fwrite(&c, 1, 1, archive);
    }

    WriteEncoded(0, file_name, file, archive);
    WriteEncoded(1, file_name, file, archive);

    fclose(archive);
    fclose(file);
}

std::string HamArc::DecodeFileName(FILE* archive) {
    std::string decoded_file_name;

    std::vector<bool> block(kTotalBits);
    int ch = getc(archive);
    int char_bit = 0;

    int out_byte = 0;
    int out_byte_bit = 0;

    int data_bits = 0;
    while (ch != EOF && !decoded_file_name.ends_with(separator)) {

        if (char_bit == 8) {
            char_bit = 0;
            ch = getc(archive);
            if (ch == EOF) {
                break;
            }
        }

        int parity_bit = 0;
        for (int i = 0; i < kTotalBits; i++) {
            if (i == (1 << parity_bit)) {
                parity_bit++;
            }
            data_bits++;
            block[i] = ((ch >> (char_bit)) & 1);
            char_bit++;

            if (char_bit == 8 && i != kTotalBits - 1) {
                char_bit = 0;
                ch = getc(archive);
                if (ch == EOF) {
                    break;
                }
            }
        }

        ValidateBlock(block, kParityBits, kTotalBits);

        parity_bit = 0;
        for (int i = 1; i < kTotalBits; i++) {
            if (i == (1 << parity_bit)) {
                parity_bit++;
                continue;
            }
            out_byte += (block[i] << out_byte_bit);
            out_byte_bit++;
            if (out_byte_bit == 8) {
                out_byte_bit = 0;
                decoded_file_name += (char) out_byte;
                if ((char) out_byte == separator) {
                    out_byte = 0;
                    break;
                }
                out_byte = 0;
            }
        }
        std::fill(block.begin(), block.end(), 0);
    }

    decoded_file_name.pop_back();
    return decoded_file_name;
}

void HamArc::DecodeFile(FILE* archive, const std::string& FILE_NAME, uint64_t total_offset) {
    std::string out_path = this->path.substr(0, this->path.length() - 3);
    if (!std::filesystem::is_directory(out_path)) {
        std::filesystem::create_directory(out_path);
    }

    FILE* decoded_file = fopen((out_path + "\\" + FILE_NAME).c_str(), "wb");

    std::vector<bool> block(kTotalBits);

    int ch = getc(archive);
    int char_bit = 0;

    int out_byte = 0;
    int out_byte_bit = 0;

    while (ftello64(archive) < total_offset) {
        for (int i = 0; i < kTotalBits; i++) {
            block[i] = ((ch >> (char_bit)) & 1);
            char_bit++;
            if (char_bit == 8) {
                char_bit = 0;
                ch = getc(archive);
                if (ch == EOF) {
                    break;
                }
            }
        }

        ValidateBlock(block, kParityBits, kTotalBits);

        int parity_bit = 0;
        for (int i = 1; i < kTotalBits; i++) {
            if (i == (1 << parity_bit)) {
                parity_bit++;
                continue;
            }
            out_byte += (block[i] << out_byte_bit);
            out_byte_bit++;
            if (out_byte_bit == 8) {
                fwrite(&out_byte, 1, 1, decoded_file);
                out_byte = 0;
                out_byte_bit = 0;
            }
        }
        std::fill(block.begin(), block.end(), 0);

        if (ch == EOF) {
            break;
        }
    }

    fclose(decoded_file);
}

void HamArc::Decode(const char* file_name) {
    std::string target_file_name = GetFileNameFromPath(file_name);

    FILE* archive = fopen(this->path.c_str(), "rb");

    uint64_t archive_size = std::filesystem::file_size(this->path);

    uint64_t total_offset = 0;

    while (ftello64(archive) < archive_size) {
        uint64_t decoded_offset = GetOffsetSize(archive);
        total_offset += decoded_offset;

        std::string decoded_file_name = DecodeFileName(archive);
        if (decoded_file_name == target_file_name || target_file_name.length() == 0) {
            DecodeFile(archive, decoded_file_name, total_offset);
        }
        fseeko64(archive, total_offset, SEEK_SET);
    }

    fclose(archive);
}

void HamArc::DeleteFiles() {
    FILE* archive = fopen(this->path.c_str(), "rb");
    FILE* updated_archive = fopen((this->path + 'f').c_str(), "wb");

    uint64_t archive_size = std::filesystem::file_size(this->path);

    uint64_t total_offset = 0;

    while (ftello64(archive) < archive_size) {
        uint64_t decoded_offset = GetOffsetSize(archive);
        total_offset += decoded_offset;

        std::string decoded_file_name = DecodeFileName(archive);
        bool to_delete = false;
        for (std::string file_name: this->files) {
            file_name = GetFileNameFromPath(file_name.c_str());
            if (decoded_file_name == file_name) {
                to_delete = true;
                break;
            }
        }
        if (!to_delete) {
            // Write data to updated_archive
            fseeko64(archive, total_offset - decoded_offset, SEEK_SET);
            while (ftello64(archive) < total_offset) {
                int c = getc(archive);
                fwrite(&c, 1, 1, updated_archive);
            }
        }
        fseeko64(archive, total_offset, SEEK_SET);
    }
    fclose(archive);
    fclose(updated_archive);
    std::filesystem::remove(this->path);
    std::filesystem::rename((this->path + 'f'), this->path);
}

void HamArc::Concatenate() {
    FILE* merged_archive = fopen(this->path.c_str(), "wb");
    for (const auto& file_name: this->files) {
        FILE* old_archive = fopen(file_name.c_str(), "rb");
        int c;
        while ((c = getc(old_archive)) != EOF) {
            fwrite(&c, 1, 1, merged_archive);
        }
        fclose(old_archive);
    }
    fclose(merged_archive);
}

void HamArc::List() {
    FILE* archive = fopen(this->path.c_str(), "rb");

    uint64_t archive_size = std::filesystem::file_size(this->path);

    uint64_t total_offset = 0;

    while (ftello64(archive) < archive_size) {
        uint64_t decoded_offset = GetOffsetSize(archive);
        total_offset += decoded_offset;

        std::cout << DecodeFileName(archive) << '\n';

        fseeko64(archive, total_offset, SEEK_SET);
    }
    fclose(archive);
}
