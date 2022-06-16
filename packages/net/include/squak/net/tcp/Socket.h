#pragma once
#include <WinSock2.h>
#include <string>
#include <optional>

namespace net {
namespace tcp {

class Socket {
private:
  SOCKET socket;

public:
  Socket(bool blocking = true)
    : socket(::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) {
    u_long ioMode = blocking ? 0 : 1;
    ioctlsocket(this->socket, FIONBIO, &ioMode);
  }
  Socket(SOCKET socket) : socket(socket) {}

  void Connect(std::string address, uint16_t port);
  void Bind(std::string address, uint16_t port);
  void Listen();
  std::optional<Socket> Accept();
  void Send(const char* buffer, size_t length);
  size_t Read(char* buffer, size_t length);
  void Send(std::string&& buffer) { this->Send(buffer.c_str(), buffer.size()); }
};

};
};