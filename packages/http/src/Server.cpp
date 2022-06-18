#include "utility.h"
#include <squak/http.h>
#include <squak/http/Server.h>

namespace http {

void Server::Listen(std::string address, uint32_t port) {
  this->socket.Bind(address.c_str(), port);
  this->socket.Listen();

  this->threads.Run([this]() {
    while (!this->threads.IsTerminated()) {
      std::unique_ptr<net::tcp::Socket> accepted = socket.Accept();
      if (accepted == nullptr) {
        _sleep(1000);
        continue;
      }

      // todo - investigate move only capture impls as std::function alternative
      net::tcp::Socket* raw = accepted.release();
      this->threads.Run([this, raw]() {
        std::unique_ptr<net::tcp::Socket> socket(raw);

        Request request = Request::Read(socket);
        Response resposnse{ socket, "HTTP/1.1", "404", "Not Found" };

        this->handler(request, resposnse);
      });
    }
  });
}

};
