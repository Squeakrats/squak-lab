#include "utility.h"
#include <WinSock2.h>
#include <squak/net/Poller.h>
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

class Socket::Impl {
private:
  SOCKET socket;
  OnDataHandler onData = []() {};

  void StartPolling() {
    net::Poller::Instance().Add(
      this->socket, [this](Poller::EventType event) { this->onData(); });
  }

  void StopPolling() {
    if (this->socket != -1) {
      net::Poller::Instance().Remove(this->socket);
    }
  }

public:
  Impl(bool blocking)
    : socket(::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) {
    u_long ioMode = blocking ? 0 : 1;
    ioctlsocket(this->socket, FIONBIO, &ioMode);
    this->StartPolling();
  }

  Impl(SOCKET socket) : socket(socket) { this->StartPolling(); }
  Impl(const Socket& socket) = delete;
  Impl(Socket&& socket) = delete;

  ~Impl() { this->StopPolling(); }

  void Connect(std::string address, uint16_t port) {
    sockaddr_in sockaddr = toSocketAddress(address, port);
    Assert(
      connect(this->socket, (struct sockaddr*)&sockaddr, sizeof(sockaddr)) == 0,
      "failed to connect");
  }

  void Bind(std::string address, uint16_t port) {
    sockaddr_in sockaddr = toSocketAddress(address.c_str(), port);
    Assert(bind(this->socket, (struct sockaddr*)&sockaddr, sizeof(sockaddr)) ==
             0,
           "failed to bind");
  }

  void Listen() { Assert(listen(this->socket, 10) == 0, "failed to listen"); }

  std::unique_ptr<Socket> Accept() {
    SOCKET accepted = accept(this->socket, nullptr, nullptr);

    if (accepted != -1) {
      u_long iMode = 0;
      ioctlsocket(accepted, FIONBIO, &iMode);

      return std::make_unique<Socket>(std::make_unique<Impl>(accepted));
    }

    return nullptr;
  }

  void Send(const char* buffer, size_t length) {
    while (length > 0) {
      int sent = send(this->socket, buffer, length, 0);
      Assert(sent > 0, "unable to send data");

      buffer += sent;
      length -= sent;
    }
  }

  void Send(std::string&& buffer) { this->Send(buffer.c_str(), buffer.size()); }

  size_t Read(char* buffer, size_t length) {
    int read = recv(this->socket, buffer, length, 0);
    if (read < 0) {
      int error = WSAGetLastError();
      Panic(std::string("read failed: ") + std::to_string(error));
    }

    return read;
  }

  void OnData(OnDataHandler onData) { this->onData = onData; }
};

void Socket::Connect(std::string address, uint16_t port) {
  this->impl->Connect(address, port);
}

void Socket::Bind(std::string address, uint16_t port) {
  this->impl->Bind(address, port);
}

void Socket::Listen() {
  this->impl->Listen();
}

std::unique_ptr<Socket> Socket::Accept() {
  return this->impl->Accept();
}

void Socket::Send(const char* buffer, size_t length) {
  this->impl->Send(buffer, length);
}

void Socket::Send(std::string&& buffer) {
  this->impl->Send(std::move(buffer));
}

size_t Socket::Read(char* buffer, size_t length) {
  return this->impl->Read(buffer, length);
}

Socket::Socket(bool blocking) {
  this->impl = std::make_unique<Socket::Impl>(blocking);
}

Socket::Socket(std::unique_ptr<Socket::Impl>&& impl) {
  this->impl = std::move(impl);
}

Socket::~Socket() {}

void Socket::OnData(OnDataHandler handler) {
  this->impl->OnData(handler);
}

};
};