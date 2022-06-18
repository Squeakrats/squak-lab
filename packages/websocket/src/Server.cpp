#include <squak/compression/base64.h>
#include <squak/compression/sha1.h>
#include <squak/websocket/Server.h>

namespace websocket {

void Server::Handler(http::Request& request, http::Response& response) {
  if (request.headers["Connection"] != "Upgrade" ||
      request.headers["Upgrade"] != "websocket") {
    response.Write();

    return;
  }

  if (this->connections.size() >= 10) {
    response.Write();

    return;
  }

  // RFC4122
  std::array<uint8_t, 20> sha1 =
    compression::sha1(request.headers["Sec-WebSocket-Key"] +
                      "258EAFA5-E914-47DA-95CA-C5AB0DC85B11");
  std::string secAccept = compression::base64::encode(sha1.data(), sha1.size());

  response.code = "101";
  response.phrase = "Switching Protocols";
  response.headers = { { "Upgrade", "websocket" },
                       { "Connection", "Upgrade" },
                       { "Sec-WebSocket-Accept", secAccept } };
  response.Write();

  uint64_t socketId = this->nextSocketId++;
  this->connections.insert(std::make_pair(
    socketId, std::make_unique<Socket>(std::move(response.socket))));

  this->connections[socketId]->OnClose([this, socketId]() {
      this->connections.erase(socketId);
  });

  this->onConnection(*this->connections[socketId]);
}

void Server::Listen(std::string address, uint32_t port) {
  this->server.Listen(address, port);
}

};
