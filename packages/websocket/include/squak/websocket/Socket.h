#pragma once
#include <squak/net/tcp/Socket.h>
#include <thread>
#include <functional>

namespace websocket {

using OnMessage = std::function<void(std::string)>;

class Socket {
  net::tcp::Socket socket;
  OnMessage onMessage{};
  std::thread thread{};

public:
  Socket(net::tcp::Socket socket);
  Socket(Socket&&) = default;
  ~Socket() { this->thread.join(); };

  void OnMessage(OnMessage onMessage) { this->onMessage = onMessage; }
};

}