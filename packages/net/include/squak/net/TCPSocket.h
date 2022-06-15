#pragma once
#include <string>
#include <WinSock2.h>

namespace net {

class TCPSocket {
private:
  SOCKET socket;

public:
  TCPSocket() : socket(::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) {}
  TCPSocket(SOCKET socket) : socket(socket) {}

  void Connect(std::string address, uint16_t port);
  void Bind(std::string address, uint16_t port);
  void Listen();
  TCPSocket Accept();
  void Send(const char* buffer, size_t length);
  size_t Read(char* buffer, size_t length);
  void Send(std::string&& buffer) { this->Send(buffer.c_str(), buffer.size()); }
};

};