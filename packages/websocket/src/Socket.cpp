#include "utility.h"
#include <array>
#include <squak/websocket/Socket.h>

namespace websocket {

Socket::Socket(net::tcp::Socket socket) : socket(socket) {
  this->thread = std::thread{ [this]() {
    while (true) {
      uint8_t bytes2[2]{};
      size_t read = this->socket.Read((char*)bytes2, 2);
      if (read <= 0) {
        break;
      }

      bool fin = (bytes2[0] & 0b10000000) != 0;
      uint8_t opCode = bytes2[0] & 0b1111;
      bool masked = (bytes2[1] & 0b10000000) != 0;
      uint8_t length = bytes2[1] & 0b01111111;

      Assert(fin == 1, "multi frame length");
      Assert(opCode == 1, "multi frame length");
      Assert(length <= 126, "multi frame length");
      Assert(masked, "data from clients must be masked");

      uint8_t mask[4]{};
      read = this->socket.Read((char*)mask, 4);
      if (read <= 0) {
        break;
      }

      std::unique_ptr<uint8_t[]> payload = std::make_unique<uint8_t[]>(length);
      read = this->socket.Read((char*)payload.get(), length);
      if (read <= 0) {
        break;
      }

      std::string message{};
      message.resize(length);

      for (size_t i = 0; i < length; i++) {
        message[i] = payload[i] ^ mask[i % 4];
      }

      onMessage(message);
    }
  } };
}

};