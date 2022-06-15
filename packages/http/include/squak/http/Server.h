#pragma once
#include <squak/net/TCPSocket.h>

namespace http {

class Server {
private:
  net::TCPSocket socket{};

public:
  void Listen(std::string address, uint32_t port);
};

};