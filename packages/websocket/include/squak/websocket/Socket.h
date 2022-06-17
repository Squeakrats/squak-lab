#pragma once
#include <squak/net/tcp/Socket.h>
#include <thread>

namespace websocket {

class Socket {
  net::tcp::Socket socket;

public:
  Socket(net::tcp::Socket socket);
  Socket(Socket&&) = default; 

};

}