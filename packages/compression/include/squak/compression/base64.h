#pragma once
#include <stdint.h>
#include <string>

namespace compression {
namespace base64 {

std::string encode(uint8_t* data, size_t length);

};
};