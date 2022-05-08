#pragma once
#include <memory>

class Buffer {
public:
    size_t size;
    uint8_t* data;
 
    Buffer(size_t size) : size(size), data(reinterpret_cast<uint8_t*>(malloc(size))) {}
    Buffer(Buffer&& source) {
        this->size = source.size;
        this->data = source.data;

        source.size = 0;
        source.data = nullptr;
    }

    Buffer(const Buffer&) = delete;
    Buffer& operator=(const Buffer&) = delete;

    ~Buffer() {
        if (data != nullptr) {
            delete data;
        }
    }
};
