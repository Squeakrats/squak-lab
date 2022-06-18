#include "utility.h"
#include <array>
#include <squak/websocket/Socket.h>

namespace websocket {

void Socket::Poll() {
  size_t read = this->socket->Read((char*)this->buffer.data() + this->buffered,
                                   this->buffer.size() - this->buffered);

  if (read == 0) {
    this->Trigger(this->onClose);
    return;
  }

  this->buffered += read;

  if (this->buffered < 2) {
    // missing fin, opCode & length
    return;
  }

  bool fin = (this->buffer[0] & 0b10000000) != 0;
  uint8_t opCode = this->buffer[0] & 0b1111;
  bool masked = (this->buffer[1] & 0b10000000) != 0;
  uint8_t length = this->buffer[1] & 0b01111111;

  Assert(fin == 1, "multi frame length");
  Assert(length <= 126, "multi frame length");
  Assert(masked, "data from clients must be masked");

  if (this->buffered < 6) {
    // missing mask
    return;
  }
  uint8_t* mask = this->buffer.data() + 2;

  if (this->buffered < 6 + length) {
    // missing payload
    return;
  }

  uint8_t* payload = this->buffer.data() + 6;

  if (opCode == 1) {
    std::string message{};
    message.resize(length);

    for (size_t i = 0; i < length; i++) {
      message[i] = payload[i] ^ mask[i % 4];
    }

    this->Trigger(this->onMessage, message);
  } else if (opCode == 8) {
    this->Trigger(this->onClose);

    return;
  } else {
    Panic("unhandled op");
  }

  this->buffered = 0;
}

Socket::Socket(std::unique_ptr<net::tcp::Socket> socket)
  : socket(std::move(socket)) {
  this->socket->OnData([this]() { this->Poll(); });
}

};