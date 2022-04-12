#pragma once
#include "json.h"
#include <stdint.h>

namespace gltf {

namespace binary {

const uint32_t GLTF = 0x46546C67;
const uint32_t JSON = 0x4E4F534A;
const uint32_t BIN = 0x004E4942;

};

json::Object Parse(const std::vector<uint8_t>& buffer);

};