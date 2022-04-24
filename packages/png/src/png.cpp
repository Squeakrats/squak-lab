#include "png.h"
#include <array>
#include <string>
#include "utility.h"
#include "ByteStream.h"
#include <iostream>
#include "compression.h"
#include <cmath>

namespace png {

std::array<uint8_t, 8> PNG_HEADER = { 137, 80, 78, 71, 13, 10, 26, 10 };
std::array<uint8_t, 4> CHUNK_TYPE_IHDR = { 73, 72, 68, 82 };
std::array<uint8_t, 4> CHUNK_TYPE_IDAT = { 73, 68, 65, 84 };
std::array<uint8_t, 4> CHUNK_TYPE_IEND = { 73, 69, 78, 68 };

IHDR parseIHDR(ByteStream& stream) {
    Assert(stream.readUInt32() == 13, "Invalid IHDR size");

    std::array<uint8_t, 4> type;
    stream.read(type.data(), 4);
    Assert(type == CHUNK_TYPE_IHDR, "Invalid IHDR type");

    IHDR chunk{};
    chunk.width = stream.readUInt32();
    chunk.height = stream.readUInt32();
    chunk.bitDepth = stream.readUInt8();
    chunk.colorType = stream.readUInt8();
    chunk.compressionMethod = stream.readUInt8();
    chunk.filterMethod = stream.readUInt8();
    chunk.interlaceMethod = stream.readUInt8();

    Assert(chunk.compressionMethod == 0, "Invalid compression method");
    Assert(chunk.filterMethod == 0, "Invalid filter method");
    Assert(chunk.interlaceMethod == 0, "Invalid interlace method");
    Assert(chunk.bitDepth == 8, "Invalid bit depth");
    Assert(chunk.colorType == COLOR_TYPE_RGB || chunk.colorType == COLOR_TYPE_RGBA, "Invalid color type");

    stream.ignore(4); // ignore crc

    return chunk;
}

void add(uint8_t* a, uint8_t* b, size_t pixelSize) {
    for (size_t i = 0; i < pixelSize; i++) {
        a[i] = a[i] + b[i];
    }
}

void avg_inverse(uint8_t* dest, uint8_t* above, uint8_t* left, size_t pixelSize) {
    for (size_t i = 0; i < pixelSize; i++) {
        uint16_t aboveValue = static_cast<uint16_t>(above[i]);
        uint16_t leftValue = static_cast<uint16_t>(left[i]);
        uint16_t value = static_cast<uint16_t>(dest[i]);
        dest[i] = value + static_cast<uint8_t>(std::floor(static_cast<double>(leftValue + aboveValue) / 2.0));
    }
}

// https://www.w3.org/TR/PNG-Filters.html
uint8_t PaethPredictor(uint8_t a, uint8_t b, uint8_t c) {
    int64_t p = static_cast<int64_t>(a) + static_cast<int64_t>(b) - static_cast<int64_t>(c);
    int64_t pa = std::abs(p - a);
    int64_t pb = std::abs(p - b);
    int64_t pc = std::abs(p - c);

    if (pa <= pb && pa <= pc) {
        return a;
    }
    else if (pb <= pc) { 
        return b;
    }
    else {
        return c;
    }
}

void Paeth_inverse(uint8_t* dest, uint8_t* above, uint8_t* left, uint8_t* upperLeft, size_t pixelSize) {
    for (size_t i = 0; i < pixelSize; i++) {
        dest[i] = dest[i] + PaethPredictor(left[i], above[i], upperLeft[i]);
    }
}

PNG parse(std::vector<uint8_t>& buffer) {
    ByteStream stream(buffer);

    std::array<uint8_t, 8> pngHeader;
    stream.read(pngHeader.data(), 8);
    Assert(pngHeader == PNG_HEADER, "invalid png header");

    IHDR header = parseIHDR(stream);
    std::vector<uint8_t> data{};

    while (true) {
        uint32_t chunkSize = stream.readUInt32();
        std::array<uint8_t, 4> chunkType;
        stream.read(chunkType.data(), 4);

        if (chunkType == CHUNK_TYPE_IDAT) {
            size_t dataSize = data.size();
            data.resize(dataSize + chunkSize);
            stream.read(data.data() + dataSize, chunkSize);
            stream.ignore(4); // ignore crc
        }
        else if (chunkType == CHUNK_TYPE_IEND) {
            break;
        }
        else {
            stream.ignore(chunkSize + 4ll);
        }
    }

    std::vector<uint8_t> inflated = inflate(data);

    size_t pixelSize = (header.colorType == COLOR_TYPE_RGB) ? 3 : 4;
    Assert(inflated.size() == header.width * header.height * pixelSize + header.height, "unexpected number of bytes");

    for (size_t line = 0; line < header.height; line++) {
        uint8_t* lineStart = inflated.data() + (header.width * pixelSize + 1ll) * line;
        uint8_t filter = lineStart[0];
        switch (filter) {
            case 0: // None
                break;
            case 1: { // Sub
                for (size_t x = 1; x < header.width; x++) {
                    add(lineStart + 1 + x * pixelSize, lineStart + 1 + (x-1) * pixelSize, pixelSize);
                }
                break;
            }   
            case 2: { // Up
                if (line == 0) {
                    break;
                }

                uint8_t* lineAbove = inflated.data() + (header.width * pixelSize + 1ll) * (line - 1);
                for (size_t x = 0; x < header.width; x++) {
                    add(lineStart + 1 + x * pixelSize, lineAbove + 1 + x * pixelSize, pixelSize);
                }
                break;
            }
            case 3: { // Up
                uint8_t* lineAbove = inflated.data() + (header.width * pixelSize + 1ll) * (line - 1);
                uint8_t zero[] = { 0, 0, 0, 0 };

                for (size_t x = 0; x < header.width; x++) {
                    uint8_t* above = (line != 0) ? lineAbove + 1 + x * pixelSize : zero;
                    uint8_t* left = (x != 0) ? lineStart + 1 + (x - 1) * pixelSize : zero;
                    avg_inverse(lineStart + 1 + x * pixelSize, above, left, pixelSize);
                }
                break;
            }
            case 4: { // Paeth
                uint8_t* lineAbove = inflated.data() + (header.width * pixelSize + 1ll) * (line - 1);
                uint8_t zero[] = { 0, 0, 0, 0 };

                for (size_t x = 0; x < header.width; x++) {
                    uint8_t* above = (line != 0) ? lineAbove + 1 + x * pixelSize : zero;
                    uint8_t* left = (x != 0) ? lineStart + 1 + (x - 1) * pixelSize : zero;
                    uint8_t* upperLeft = (x != 0 && line != 0) ? lineAbove + 1 + (x - 1) * pixelSize : zero;
                    Paeth_inverse(lineStart + 1 + x * pixelSize, above, left, upperLeft, pixelSize);
                }
                break;
            }
            default:
                Assert(false, "unsupported filter");
        }
    }

    std::vector<uint8_t> pixels{};

    uint8_t* position = inflated.data();
    for (size_t y = 0; y < header.height; y++) {
        position++;
        for (size_t x = 0; x < header.width * pixelSize; x++) {
            pixels.push_back(*(position++));
        }
    }

    return PNG{ header, std::move(pixels) };
}

};
