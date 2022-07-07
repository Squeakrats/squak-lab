#include "utility.h"
#include <squak/http.h>
#include <squak/http/Server.h>

namespace http {

void Server::Listen(std::string address, uint32_t port) {
  this->socket.Bind(address.c_str(), port);
  this->socket.Listen();
  this->socket.OnData([this] {
    net::tcp::Socket* raw = this->socket.Accept().release();

    raw->OnData([this, raw] {
      std::unique_ptr<net::tcp::Socket> accepted(raw);

      Request request = Request::Read(accepted);
      Response response{ accepted, "HTTP/1.1", "404", "Not Found" };

      this->handler(request, response);
    });
  });
}

};
