#pragma once
#include <string>
#include <array>

namespace compression {

std::array<uint8_t, 20> sha1(std::string message);

};