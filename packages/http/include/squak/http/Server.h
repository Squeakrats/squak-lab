#pragma once
#include <squak/http.h>
#include <squak/core/ThreadPool.h>
#include <squak/net/tcp/Socket.h>

namespace http {

using RequestHandler =
  std::function<void(Request& request, Response& response)>;

class Server {
private:
  net::tcp::Socket socket{false};
  RequestHandler handler;
  ThreadPool threads{10};

public:
  Server(RequestHandler handler) : handler(handler) {}

  void Listen(std::string address, uint32_t port);
};

};