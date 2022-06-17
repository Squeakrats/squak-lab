#include "utility.h"
#include <squak/net/tcp/Socket.h>

namespace net {
namespace tcp {

sockaddr_in toSocketAddress(std::string address, uint16_t port) {
  sockaddr_in sockaddr{};
  sockaddr.sin_family = AF_INET;
  sockaddr.sin_addr.s_addr = inet_addr(address.c_str());
  sockaddr.sin_port = htons(port);

  return sockaddr;
}

void Socket::Connect(std::string address, uint16_t port) {
  sockaddr_in sockaddr = toSocketAddress(address, port);
  Assert(connect(this->socket, (struct sockaddr*)&sockaddr, sizeof(sockaddr)) ==
           0,
         "failed to connect");
}

void Socket::Bind(std::string address, uint16_t port) {
  sockaddr_in sockaddr = toSocketAddress(address.c_str(), port);
  Assert(bind(this->socket, (struct sockaddr*)&sockaddr, sizeof(sockaddr)) == 0,
         "failed to bind");
}

void Socket::Listen() {
  Assert(listen(this->socket, 10) == 0, "failed to listen");
}

void Socket::Send(const char* buffer, size_t length) {
  while (length > 0) {
    int sent = send(this->socket, buffer, length, 0);
    Assert(sent > 0, "unable to send data");

    buffer += sent;
    length -= sent;
  }
}

std::optional<Socket> Socket::Accept() {
  SOCKET accepted = accept(this->socket, nullptr, nullptr);

  if (accepted != -1) {
    u_long iMode = 0;
    ioctlsocket(accepted, FIONBIO, &iMode);

    return Socket(accepted);
  }

  return std::nullopt;
}

size_t Socket::Read(char* buffer, size_t length) {
  int read = recv(this->socket, buffer, length, 0);
  if (read < 0) {
    int error = WSAGetLastError();
    Panic(std::string("read failed: ") + std::to_string(error) );
  }

  return read;
}

};
};