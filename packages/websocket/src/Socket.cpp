#include <squak/websocket/Socket.h>
#include <array>

namespace websocket {

Socket::Socket(net::tcp::Socket socket) : socket(socket) {}

};