#pragma once
#include <squak/net/tcp/Socket.h>

namespace http {

class Server {
private:
  net::tcp::Socket socket{};

public:
  void Listen(std::string address, uint32_t port);
};

};