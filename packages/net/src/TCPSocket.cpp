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
    "unable to connect");
}

void TCPSocket::Send(const char* buffer, size_t length) {
  while (length > 0) {
    int sent = send(this->socket, buffer, length, 0);
    Assert(sent > 0, "unable to send data");

    buffer += sent;
    length -= sent;
  }
}

size_t TCPSocket::Read(char* buffer, size_t length) {
  int read = recv(this->socket, buffer, length, 0);
  Assert(read >= 0, "read failed");

  return read;
}

};