#include <squak/websocket/Server.h>
#include <squak/compression/sha1.h>
#include <squak/compression/base64.h>

namespace websocket {

void Server::Handler(http::Request& request, http::Response& response) {
  if (request.headers["Connection"] != "Upgrade" || request.headers["Upgrade"] !=
      "websocket") {
    response.Write();

    return;
  }

  // RFC4122
  std::string magic = request.headers["Sec-WebSocket-Key"] +
                      "258EAFA5-E914-47DA-95CA-C5AB0DC85B11";
  std::array<uint8_t, 20> sha1 = compression::sha1(magic);
  std::string secWebsocketAccept =
    compression::base64::encode(sha1.data(), sha1.size());

  response.code = "101";
  response.phrase = "Switching Protocols";
  response.headers = { { "Upgrade", "websocket" },
                       { "Connection", "Upgrade" },
                       { "Sec-WebSocket-Accept", secWebsocketAccept } };
  response.Write();
}

void Server::Listen(std::string address, uint32_t port) {
  this->server.Listen(address, port);
}

};
