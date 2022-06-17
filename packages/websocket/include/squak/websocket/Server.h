#pragma once
#include <squak/http/Server.h>
#include <squak/websocket/Socket.h>

namespace websocket {

using OnConnectionHandler = std::function<void(Socket&)>;

class Server {
private:
  http::Server server;
  OnConnectionHandler onConnection{};
  std::vector<std::unique_ptr<Socket>> connections{};

  void Handler(http::Request& request, http::Response& response);

public:
  Server()
    : server([this](http::Request& request, http::Response& response) {
      this->Handler(request, response);
    }) {}

  void Listen(std::string address, uint32_t port);
  void OnConnection(OnConnectionHandler onConnection) {
    this->onConnection = onConnection;
  }
};

};