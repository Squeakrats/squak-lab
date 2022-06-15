#pragma once
#include <string>
#include <WinSock2.h>

namespace net {
namespace tcp {

class Socket {
private:
  SOCKET socket;

public:
  Socket() : socket(::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) {}
  Socket(SOCKET socket) : socket(socket) {}

  void Connect(std::string address, uint16_t port);
  void Bind(std::string address, uint16_t port);
  void Listen();
  Socket Accept();
  void Send(const char* buffer, size_t length);
  size_t Read(char* buffer, size_t length);
  void Send(std::string&& buffer) { this->Send(buffer.c_str(), buffer.size()); }
};

};
};