#pragma once
#include <map>
#include <squak/net/tcp/Socket.h>
#include <string>

namespace http {

struct Response {
  std::unique_ptr<net::tcp::Socket>& socket;
  std::string version{};
  std::string code{};
  std::string phrase{};
  std::map<std::string, std::string> headers{};
  std::string body{};

  static Response Read(std::unique_ptr<net::tcp::Socket> socket);
  void Write();
};

struct Request {
  std::unique_ptr<net::tcp::Socket>& socket;
  std::string method{};
  std::string uri{};
  std::string version{};
  std::map<std::string, std::string> headers{};
  std::string body{};

  static Request Read(std::unique_ptr<net::tcp::Socket>& socket);
  void Write();
};

std::string fetch(std::string address, uint16_t port);

};