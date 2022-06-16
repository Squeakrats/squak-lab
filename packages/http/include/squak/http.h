#pragma once
#include <map>
#include <squak/net/tcp/Socket.h>
#include <string>

namespace http {

struct Response {
  std::string version{};
  std::string code{};
  std::string phrase{};
  std::map<std::string, std::string> headers{};
  std::string body{};

  static Response Read(net::tcp::Socket& socket);
  void Write(net::tcp::Socket& socket);
};

struct Request {
  std::string method{};
  std::string uri{};
  std::string version{};
  std::map<std::string, std::string> headers{};
  std::string body{};

  static Request Read(net::tcp::Socket& socket);
  void Write(net::tcp::Socket& socket);
};

std::string fetch(std::string address, uint16_t port);

};