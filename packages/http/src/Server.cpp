#include <squak/http/Server.h>
#include <squak/http.h>
#include "utility.h"

namespace http {

void Server::Listen(std::string address, uint32_t port) {
  this->socket.Bind(address.c_str(), port);
  this->socket.Listen();

  while (true) {
    net::TCPSocket accepted = socket.Accept();
    Response response = http::Response::Read(accepted);
    Assert(response.headers["Upgrade"] == "websocket", "unexpected upgrade");
  }
}

};
