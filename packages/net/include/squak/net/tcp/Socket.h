#pragma once
#include <WinSock2.h>
#include <memory>
#include <squak/net/Poller.h>
#include <string>

namespace net {
namespace tcp {

class Socket {
public:
  using OnDataHandler = std::function<void(void)>;

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
  Socket(bool blocking = true)
    : socket(::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) {
    u_long ioMode = blocking ? 0 : 1;
    ioctlsocket(this->socket, FIONBIO, &ioMode);
    this->StartPolling();
  }
  Socket(SOCKET socket) : socket(socket) { this->StartPolling(); }
  Socket(const Socket& socket) = delete;
  Socket(Socket&& socket) = delete;
  ~Socket() { this->StopPolling(); }

  void Connect(std::string address, uint16_t port);
  void Bind(std::string address, uint16_t port);
  void Listen();
  std::unique_ptr<Socket> Accept();
  void Send(const char* buffer, size_t length);
  size_t Read(char* buffer, size_t length);
  void Send(std::string&& buffer) { this->Send(buffer.c_str(), buffer.size()); }
  void OnData(OnDataHandler onData) { this->onData = onData; }
};

};
};