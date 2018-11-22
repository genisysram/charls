// Copyright (c) Team CharLS. All rights reserved. See the accompanying "LICENSE.md" for licensed use.

#pragma once

#include <vector>
#include <fstream>
#include <cstddef>

class bmp_image final
{
public:
    struct bmp_header final
    {
        uint8_t magic[2];   // the magic number used to identify the BMP file:
                            // 0x42 0x4D (Hex code points for B and M).
                            // The following entries are possible:
                            // BM - Windows 3.1x, 95, NT, ... etc
                            // BA - OS/2 Bitmap Array
                            // CI - OS/2 Color Icon
                            // CP - OS/2 Color Pointer
                            // IC - OS/2 Icon
                            // PT - OS/2 Pointer.
        uint32_t file_size; // the size of the BMP file in bytes
        uint32_t reserved;  // reserved.
        uint32_t offset;    // the offset, i.e. starting address, of the byte where the bitmap data can be found.
    };

    struct bmp_dib_header final
    {
        uint32_t header_size;             // the size of this header (40 bytes)
        uint32_t width;                   // the bitmap width in pixels
        uint32_t height;                  // the bitmap height in pixels
        uint16_t number_planes;           // the number of color planes being used. Must be set to 1
        uint16_t depth;                   // the number of bits per pixel,which is the color depth of the image.
                                          // Typical values are 1, 4, 8, 16, 24 and 32.
        uint32_t compress_type;           // the compression method being used.
        uint32_t bmp_byte_size;           // the image size. This is the size of the raw bitmap
                                          // data (see below), and should not be confused with the file size.
        uint32_t horizontal_resolution;   // the horizontal resolution of the image. (pixel per meter)
        uint32_t vertical_resolution;     // the vertical resolution of the image. (pixel per meter)
        uint32_t number_colors;           // the number of colors in the color palette, or 0 to default to 2^depth.
        uint32_t number_important_colors; // the number of important colors used, or 0 when every color is important generally ignored.
    };


    explicit bmp_image(const char* filename)
    {
        std::ifstream input(filename, std::ios::binary);

        header = read_header(input);
        if (header.magic[0] != 0x42 || header.magic[1] != 0x4D)
            throw std::exception("Missing BMP identifier");

        dib_header = read_dib_header(input);
    }

    bmp_header header;
    bmp_dib_header dib_header;
    std::vector<std::byte> pixel_data;

private:
    static bmp_header read_header(std::istream& input)
    {
        bmp_header result{};

        input.read(reinterpret_cast<char*>(&result.magic), sizeof result.magic);
        input.read(reinterpret_cast<char*>(&result.file_size), sizeof result.file_size);
        input.read(reinterpret_cast<char*>(&result.reserved), sizeof result.reserved);
        input.read(reinterpret_cast<char*>(&result.offset), sizeof result.offset);

        return result;
    }

    static bmp_dib_header read_dib_header(std::istream& input)
    {
        bmp_dib_header result{};

        input.read(reinterpret_cast<char*>(&result.header_size), sizeof result.header_size);
        input.read(reinterpret_cast<char*>(&result.width), sizeof result.width);
        input.read(reinterpret_cast<char*>(&result.height), sizeof result.height);
        input.read(reinterpret_cast<char*>(&result.number_planes), sizeof result.number_planes);
        input.read(reinterpret_cast<char*>(&result.depth), sizeof result.depth);
        input.read(reinterpret_cast<char*>(&result.compress_type), sizeof result.compress_type);

        return result;
    }
};
