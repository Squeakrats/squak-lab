#include "utility.h"
#include <squak/http.h>
#include <squak/http/Server.h>

namespace http {

void Server::Listen(std::string address, uint32_t port) {
  this->socket.Bind(address.c_str(), port);
  this->socket.Listen();

  this->threads.Run([this]() {
    while (!this->threads.IsTerminated()) {
      std::optional<net::tcp::Socket> accepted = socket.Accept();
      if (accepted == std::nullopt) {
        _sleep(1000);
        continue;
      }

      net::tcp::Socket socket = *accepted;
      this->threads.Run([this, socket]() mutable {
        Request request = Request::Read(socket);
        Response resposnse{ socket, "HTTP/1.1", "404", "Not Found" };

        this->handler(request, resposnse);
        resposnse.Write();
      });
    }
  });
}

};
