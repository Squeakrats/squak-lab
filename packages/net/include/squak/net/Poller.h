#pragma once
#include <WinSock2.h>
#include <map>
#include <functional>

namespace net {
  
class Poller {
public:
  enum class EventType {
    Read,
    Write,
  };

  using OnEvent = std::function<void(EventType)>;

private:
  std::map<SOCKET, OnEvent> sockets{};

public:
  void Add(SOCKET socket, OnEvent onEvent);
  void Remove(SOCKET socket);
  void Poll();

  static Poller& Instance() {
    static Poller poller{};

    return poller;
  }

};

}