#pragma once
#include <string>
#include <map>
#include <squak/net/tcp/Socket.h>

namespace http {

struct Response {
    std::map<std::string, std::string> headers{};
    std::string body{};

    static Response Read(net::tcp::Socket& socket);
};

struct Request {
  std::string method{};
  std::string uri{};
  std::string version{};
  std::map<std::string, std::string> headers{};
  std::string body{};

  void Write(net::tcp::Socket& socket);
};

std::string fetch(std::string address, uint16_t port);

};