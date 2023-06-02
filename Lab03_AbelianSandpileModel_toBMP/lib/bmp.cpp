#include "bmp.h"

void CreateBMP(const char* file_path, const uint64_t* table, uint16_t height, uint16_t width, const int* indent) {
    enum SIDE {
        UP = 0, DOWN, LEFT, RIGHT
    };
    width = width - indent[LEFT] - indent[RIGHT];
    height = height - indent[UP] - indent[DOWN];

    // 2 pixels in 1 byte, but 4 bytes in chunk
    unsigned char pad[3] = {0, 0, 0}; // Filler
    const unsigned char pad_count = (4 - ((width * BITS_PER_PIXEL / 8) + (width % 2)) % 4) % 4;

    uint32_t image_size = ((width * BITS_PER_PIXEL / 8) + (width % 2)) * height + (pad_count * height);

    uint32_t file_size = (FILE_HEADER_SIZE + INFO_HEADER_SIZE + COLOR_COUNT * 4) + // Headers size
                         image_size;

    unsigned char FILE_HEADER[FILE_HEADER_SIZE] = {
            'B', 'M', // BMP file format
            static_cast<unsigned char>(file_size), // Size of BMP file in bytes
            static_cast<unsigned char>(file_size >> 8),
            static_cast<unsigned char>(file_size >> 16),
            static_cast<unsigned char>(file_size >> 24),
            0, 0, 0, 0, // Reserved (can be 0)
            FILE_HEADER_SIZE + INFO_HEADER_SIZE + COLOR_COUNT * 4, 0, 0, 0 // Offset (address where image starts)
    };

    unsigned char INFO_HEADER[INFO_HEADER_SIZE] = {
            INFO_HEADER_SIZE, 0, 0, 0, // Size of this header
            static_cast<unsigned char>(width), // BMP width in pixels
            static_cast<unsigned char>(width >> 8),
            static_cast<unsigned char>(width >> 16),
            static_cast<unsigned char>(width >> 24),
            static_cast<unsigned char>(height), // BMP height in pixels
            static_cast<unsigned char>(height >> 8),
            static_cast<unsigned char>(height >> 16),
            static_cast<unsigned char>(height >> 24),
            1, 0, // Number of color planes (MUST be 1)
            BITS_PER_PIXEL, 0, // Bits per pixel
            0, 0, 0, 0, // Compression method (0 for BI_RGB and no compression)
            static_cast<unsigned char>(image_size), // Image size. 0 can be given for BI_RGB bitmaps.
            static_cast<unsigned char>(image_size >> 8),
            static_cast<unsigned char>(image_size >> 16),
            static_cast<unsigned char>(image_size >> 24),
            0, 0, 0, 0, // Horizontal resolution
            0, 0, 0, 0, // Vertical resolution
            COLOR_COUNT, 0, 0, 0, // Number of colors in palette (0 == 2^bits_per_pixel)
            0, 0, 0, 0 // Important colors (0 when every color is important)
    };

    FILE* image_file = fopen(file_path, "wb");
    fwrite(FILE_HEADER, 1, FILE_HEADER_SIZE, image_file);
    fwrite(INFO_HEADER, 1, INFO_HEADER_SIZE, image_file);
    fwrite(COLOR_TABLE, 4, COLOR_COUNT, image_file);

    for (size_t i = indent[UP]; i < height + indent[UP]; i++) {
        unsigned char image_byte = 0;
        bool is_ready_byte = false;

        for (size_t j = indent[LEFT]; j < width + indent[LEFT]; j++) {

            unsigned char color;
            if (table[i * (width + indent[LEFT] + indent[RIGHT]) + j] == 0) {
                color = white;
            } else if (table[i * (width + indent[LEFT] + indent[RIGHT]) + j] == 1) {
                color = green;
            } else if (table[i * (width + indent[LEFT] + indent[RIGHT]) + j] == 2) {
                color = purple;
            } else if (table[i * (width + indent[LEFT] + indent[RIGHT]) + j] == 3) {
                color = yellow;
            } else {
                color = black;
            }

            if (is_ready_byte) {
                image_byte += color;
                fwrite(&image_byte, 1, 1, image_file);
                image_byte = 0;
                is_ready_byte = false;
            } else {
                image_byte = (color << 4);
                is_ready_byte = true;
            }
        }
        if (is_ready_byte) {
            fwrite(&image_byte, 1, 1, image_file);
        }
        fwrite(pad, 1, pad_count, image_file);
    }

    fclose(image_file);
}
