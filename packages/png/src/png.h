#pragma once
#include "BufferView.h"

namespace png {

const uint8_t COLOR_TYPE_RGB = 2;
const uint8_t COLOR_TYPE_RGBA = 6;

struct IHDR {
    uint32_t width;
    uint32_t height;
    uint8_t bitDepth;
    uint8_t colorType;
    uint8_t compressionMethod;
    uint8_t filterMethod;
    uint8_t interlaceMethod;
};

struct PNG {
    IHDR header;
	Buffer pixels;
};

PNG parse(BufferView view);

};