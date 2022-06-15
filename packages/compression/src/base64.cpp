#include <squak/compression/base64.h>
#include <sstream>

namespace compression {
namespace base64 {

char encoder[64] = { 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K',
                     'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V',
                     'W', 'X', 'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f', 'g',
                     'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r',
                     's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '0', '1', '2',
                     '3', '4', '5', '6', '7', '8', '9', '+', '/' };

void write_triple(std::stringstream& stream, uint8_t a, uint8_t b, uint8_t c) {}

std::string encode(std::string data) {
  uint8_t* buffer = (uint8_t*)data.data();
  size_t offset = 0;

  std::stringstream stream{};
  while (offset + 2 < data.size()) {
    uint32_t value = (static_cast<uint32_t>(buffer[offset]) << 16) |
                     (static_cast<uint32_t>(buffer[offset + 1]) << 8) |
                     static_cast<uint32_t>(buffer[offset + 2]);

    uint8_t codes[4] = { static_cast<uint8_t>((value >> 18) & 0x3F),
                         static_cast<uint8_t>((value >> 12) & 0x3F),
                         static_cast<uint8_t>((value >> 6) & 0x3F),
                         static_cast<uint8_t>((value >> 0) & 0x3F) };

    for (size_t i = 0; i < 4; i++) {
      stream << encoder[codes[i]];
    }

    offset += 3;
  }

  if (offset + 1 < data.size()) {
    uint32_t value = (static_cast<uint32_t>(buffer[offset]) << 16) |
                     (static_cast<uint32_t>(buffer[offset + 1]) << 8);

    uint8_t codes[3] = { static_cast<uint8_t>((value >> 18) & 0x3F),
                         static_cast<uint8_t>((value >> 12) & 0x3F),
                         static_cast<uint8_t>((value >> 6) & 0x3F) };

    for (size_t i = 0; i < 3; i++) {
      stream << encoder[codes[i]];
    }

    stream << "=";
  } else if (offset < data.size()) {
    uint32_t value = (static_cast<uint32_t>(buffer[offset]) << 16);

    uint8_t codes[2] = { static_cast<uint8_t>((value >> 18) & 0x3F),
                         static_cast<uint8_t>((value >> 12) & 0x3F) };

    for (size_t i = 0; i < 2; i++) {
      stream << encoder[codes[i]];
    }

    stream << "==";
  }

  return stream.str();
}

};
};