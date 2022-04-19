#pragma once
#include <vector>

class ByteStream{
    std::vector<uint8_t>& data;
    size_t pos{};

public:
    ByteStream(std::vector<uint8_t>& data) : data(data) {}

    uint32_t readUInt32() {
        uint32_t value = 
              static_cast<uint32_t>(this->data[this->pos + 0]) << 24
            | static_cast<uint32_t>(this->data[this->pos + 1]) << 16
            | static_cast<uint32_t>(this->data[this->pos + 2]) << 8
            | static_cast<uint32_t>(this->data[this->pos + 3]);

        this->pos += 4;

        return value;
    }

    void read(uint8_t* dest, size_t n) {
        for (size_t i = 0; i < n; i++) {
            dest[i] = this->data[this->pos++];
        }
    }

    uint8_t readUInt8() {
        return this->data[this->pos++];
    }

    void ignore(size_t n) {
        this->pos += n;
    }
};
