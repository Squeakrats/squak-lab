#include "utility.h"
#include <array>
#include <squak/http.h>
#include <squak/net/TCPSocket.h>
#include <sstream>
#include <vector>

namespace http {

std::string createRequest(std::string method,
                          std::string uri,
                          std::string version) {
  std::stringstream stream{};
  stream << method << " " << uri << " " << version << "\r\n";
  stream << "\r\n";

  return stream.str();
}

Response Response::Read(net::TCPSocket& socket) {
  auto readLine = [&socket]() -> std::string& {
    static std::string buffer{};
    buffer.resize(1);
    Assert(socket.Read(&buffer[buffer.size() - 1], 1) == 1, "read failed");

    while (true) {
      buffer.resize(buffer.size() + 1);
      Assert(socket.Read(&buffer[buffer.size() - 1], 1) == 1, "read failed");

      size_t tail = buffer.size() - 1;
      if (buffer[tail] == '\n' && buffer[tail - 1] == '\r') {
        return buffer;
      }
    }
  };

  bool foundStatusLine = false;
  std::map<std::string, std::string> headers{};

  while (true) {
    std::string& line = readLine();

    if (!foundStatusLine) {
      foundStatusLine = true;
      continue;
    }

    if (line.size() == 2) {
      break;
    }

    size_t split = line.find_first_of(':');
    size_t valueStart = split + 1;
    while (line[valueStart] == ' ') {
      valueStart++;
    }

    headers.insert(
      std::make_pair(line.substr(0, split),
                     line.substr(valueStart, line.size() - valueStart - 2)));
  }

  if (headers["Transfer-Encoding"] != "chunked") {
    return { headers, {} };
  }

  std::vector<char> bodyBuffer{};
  while (true) {
    std::string line = readLine();
    uint64_t chunkDataToRead = std::strtol(line.c_str(), nullptr, 16);
    if (chunkDataToRead == 0) {
      break;
    }

    size_t offset = bodyBuffer.size();
    bodyBuffer.resize(bodyBuffer.size() + chunkDataToRead);

    while (chunkDataToRead > 0) {
      size_t read = socket.Read(bodyBuffer.data() + offset, chunkDataToRead);
      Assert(read > 0, "failed to read data");
      offset += read;
      chunkDataToRead -= read;
    }

    char character;
    Assert(socket.Read(&character, 1) == 1, "read failed");
    Assert(socket.Read(&character, 1) == 1, "read failed");
  }

  return Response{ headers, std::string(bodyBuffer.begin(), bodyBuffer.end()) };
}

std::string fetch(std::string address, uint16_t port) {
  net::TCPSocket socket{};
  socket.Connect(address, port);
  socket.Send(createRequest("GET", "/", "HTTP/1.1"));

  return Response::Read(socket).body;
}

}