#pragma once
#include <string>
#include <map>

namespace http {

struct Request {
  std::string method;
  std::string uri;
  std::string version;
  std::map<std::string, std::string> headers;
};

struct Response {
  std::string version;
  std::string status;
  std::string reason;
  std::map<std::string, std::string> headers;
};

std::string fetch(std::string address, uint16_t port);

};