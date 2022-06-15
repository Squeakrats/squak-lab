#include <squak/net/TCPSocket.h>
#include "utility.h"

namespace net {

sockaddr_in toSocketAddress(std::string address, uint16_t port) {
  sockaddr_in sockaddr{};
  sockaddr.sin_family = AF_INET;
  sockaddr.sin_addr.s_addr = inet_addr(address.c_str());
  sockaddr.sin_port = htons(port);

  return sockaddr;
}

void TCPSocket::Connect(std::string address, uint16_t port) {
  sockaddr_in sockaddr = toSocketAddress(address, port);
  Assert(
    connect(this->socket, (struct sockaddr*)&sockaddr, sizeof(sockaddr)) == 0,
    "failed to connect");
}

void TCPSocket::Bind(std::string address, uint16_t port) {
  sockaddr_in sockaddr = toSocketAddress(address.c_str(), port);
  Assert(bind(this->socket, (struct sockaddr*)&sockaddr, sizeof(sockaddr)) ==
           0,
         "failed to bind");
}

void TCPSocket::Listen() {
  Assert(listen(this->socket, 10) == 0, "failed to listen");
}

void TCPSocket::Send(const char* buffer, size_t length) {
  while (length > 0) {
    int sent = send(this->socket, buffer, length, 0);
    Assert(sent > 0, "unable to send data");

    buffer += sent;
    length -= sent;
  }
}

TCPSocket TCPSocket::Accept() { 
  SOCKET accepted = accept(this->socket, nullptr, nullptr);
  Assert(accepted != -1, "failed to accept socket");
  
  return TCPSocket(accepted);
}

size_t TCPSocket::Read(char* buffer, size_t length) {
  int read = recv(this->socket, buffer, length, 0);
  Assert(read >= 0, "read failed");

  return read;
}

};