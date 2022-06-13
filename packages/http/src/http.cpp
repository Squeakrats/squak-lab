#include "utility.h"
#include <squak/http.h>
#include <squak/net/TCPSocket.h>
#include <sstream>
#include <vector>

namespace http {

std::string toString(Request&& request) {
  std::stringstream stream{};
  stream << request.method << " " << request.uri << " " << request.version
         << "\r\n";

  stream << "\r\n";

  return stream.str();
}

std::string fetch(std::string address, uint16_t port) {
  net::TCPSocket socket{};
  socket.Connect(address, port);
  socket.Send(
    toString(Request{ "GET", "/", "HTTP/1.1", { { "Host", "127.0.0.1" } } }));

  return "";
}

}