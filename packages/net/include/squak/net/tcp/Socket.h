#pragma once
#include <memory>
#include <string>
#include <functional>

namespace net {
namespace tcp {

class Socket {
public:
  using OnDataHandler = std::function<void(void)>;

private:
  class Impl;
  std::unique_ptr<Impl> impl;

public:
  Socket(bool blocking = true);
  Socket(std::unique_ptr<Impl>&& impl);

  Socket(const Socket& socket) = delete;
  Socket(Socket&& socket) = delete;

  ~Socket();

  void Connect(std::string address, uint16_t port);
  void Bind(std::string address, uint16_t port);
  void Listen();
  std::unique_ptr<Socket> Accept();

  void Send(const char* buffer, size_t length);
  void Send(std::string&& buffer);

  size_t Read(char* buffer, size_t length);

  void OnData(OnDataHandler onData);
};

};
};