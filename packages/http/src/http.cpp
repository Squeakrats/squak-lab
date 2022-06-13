#include "utility.h"
#include <squak/http.h>
#include <squak/net/TCPSocket.h>
#include <sstream>
#include <vector>
#include <array>

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

  size_t responseLength = 0;
  std::array<char, 2048> buffer{};
  size_t read = socket.Read(buffer.data(), buffer.size());
  for (size_t i = 3; i < read; i++) {
    if (buffer[i - 3] == '\r' && buffer[i - 2] == '\n' &&
        buffer[i - 1] == '\r' && buffer[i - 0] == '\n') {
      responseLength = i;
      break;
    }
  }

  Assert(responseLength > 0, "failed to parse http header from first read");
  std::string response(buffer.begin(), buffer.begin() + responseLength);

  return response;
}

}