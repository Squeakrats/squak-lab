#pragma once
#include <vector>
#include <cmath>

struct BitStream {
    std::vector<uint8_t>& buffer;
    uint64_t position;

    BitStream(std::vector<uint8_t>& buffer, uint64_t position) : buffer(buffer), position(position) {}

    uint8_t ReadBit() {
        uint64_t base = static_cast<uint64_t>(std::floor(position / 8));
        uint8_t offset = position % 8;

        position++;

        return (buffer[base] & (1 << offset)) != 0;
    }

    size_t Read(size_t n) {
        size_t value{};
        for (size_t i = 0; i < n; i++) {
            value = (value << 1) + this->ReadBit();
        }

        return value;
    }
};