#pragma once
#include <string>
#include <map>
#include <squak/net/TCPSocket.h>

namespace http {

struct Response {
    std::map<std::string, std::string> headers{};
    std::string body{};

    static Response Read(net::TCPSocket& socket);
};

std::string fetch(std::string address, uint16_t port);

};