#pragma once
#include <squak/net/tcp/Socket.h>
#include <thread>
#include <functional>

namespace websocket {

using OnMessage = std::function<void(std::string)>;

class Socket {
  std::unique_ptr<net::tcp::Socket> socket;
  OnMessage onMessage{};
  std::thread thread{};

  void Poll();

public:
  Socket(std::unique_ptr<net::tcp::Socket> socket);
  Socket(const Socket&) = delete;
  Socket(Socket&&) = delete;
  ~Socket() { this->thread.join(); };

  void OnMessage(OnMessage onMessage) { this->onMessage = onMessage; }
};

}