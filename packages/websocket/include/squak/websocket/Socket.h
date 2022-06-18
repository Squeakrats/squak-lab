#pragma once
#include <array>
#include <functional>
#include <squak/net/tcp/Socket.h>

namespace websocket {

using OnMessage = std::function<void(std::string)>;
using OnClose = std::function<void()>;

class Socket {
  std::unique_ptr<net::tcp::Socket> socket;
  std::vector<OnMessage> onMessage{};
  std::vector<OnClose> onClose{};
  std::array<uint8_t, 4096> buffer{};
  size_t buffered = 0;

  void Poll();

  template<typename T>
  void AddEventListener(std::vector<T>& listeners, T listener) {
    listeners.push_back(listener);
  }

  template<typename T, typename ...Arguments>
  void Trigger(std::vector<T>& listeners, Arguments... arguments) {
    std::vector<T> l = listeners;
    for (auto& listener : l) {
      listener(arguments...);
    }
  }

public:
  Socket(std::unique_ptr<net::tcp::Socket> socket);
  Socket(const Socket&) = delete;
  Socket(Socket&&) = delete;

  void OnMessage(OnMessage onMessage) {
    AddEventListener(this->onMessage, onMessage);
  }
  void OnClose(OnClose onClose) {
    AddEventListener(this->onClose, onClose);
  }
};

}