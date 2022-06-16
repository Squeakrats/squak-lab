#include "utility.h"
#include <squak/compression/base64.h>
#include <squak/compression/sha1.h>
#include <squak/http.h>
#include <squak/http/Server.h>

namespace http {

void Server::Listen(std::string address, uint32_t port) {
  this->socket.Bind(address.c_str(), port);
  this->socket.Listen();

  while (true) {
    net::tcp::Socket accepted = socket.Accept();
    Request request = http::Request::Read(accepted);
    Assert(request.headers["Upgrade"] == "websocket", "unexpected upgrade");

    // RFC4122
    std::string magic = request.headers["Sec-WebSocket-Key"] +
                        "258EAFA5-E914-47DA-95CA-C5AB0DC85B11";
    std::array<uint8_t, 20> sha1 = compression::sha1(magic);
    std::string secWebsocketAccept = compression::base64::encode(sha1.data(), sha1.size());

    Response response{ "HTTP/1.1",
                       "101",
                       "Switching Protocols",
                       { { "Upgrade", "websocket" },
                         { "Connection", "Upgrade" },
                         { "Sec-WebSocket-Accept", secWebsocketAccept } } };
    response.Write(accepted);
  }
}

};
