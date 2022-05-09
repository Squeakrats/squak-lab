#pragma once
#include <cmath>
#include <vector>

struct BitStream {
  Buffer& buffer;
  uint64_t byteOffset;
  uint8_t bitOffset;

  BitStream(Buffer& buffer, uint64_t position)
    : buffer(buffer)
    , byteOffset(static_cast<uint64_t>(std::floor(position / 8)))
    , bitOffset(position % 8) {}

  uint8_t ReadBit() {
    uint8_t bit =
      (this->buffer.data[this->byteOffset] & (1 << this->bitOffset)) != 0;

    this->bitOffset++;
    if (this->bitOffset == 8) {
      this->byteOffset++;
      this->bitOffset = 0;
    }

    return bit;
  }

  size_t ReadLiteral(size_t n) {
    size_t value{};
    for (size_t i = 0; i < n; i++) {
      value |= static_cast<size_t>(this->ReadBit()) << i;
    }

    return value;
  }
};