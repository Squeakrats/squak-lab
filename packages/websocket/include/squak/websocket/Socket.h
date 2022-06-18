#pragma once
#include <squak/net/tcp/Socket.h>
#include <functional>
#include <array>

namespace websocket {

using OnMessage = std::function<void(std::string)>;

class Socket {
  std::unique_ptr<net::tcp::Socket> socket;
  OnMessage onMessage{};
  std::array<uint8_t, 4096> buffer{};
  size_t buffered = 0;

  void Poll();

public:
  Socket(std::unique_ptr<net::tcp::Socket> socket);
  Socket(const Socket&) = delete;
  Socket(Socket&&) = delete;

  void OnMessage(OnMessage onMessage) { this->onMessage = onMessage; }
};

}