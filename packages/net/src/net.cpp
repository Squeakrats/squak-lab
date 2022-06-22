#include "utility.h"
#include <squak/net.h>
#ifndef EMSCRIPTEN
#include <squak/net/Poller.h>
#endif

namespace net {

void Init() {
#ifndef EMSCRIPTEN
  WSAData wsaData{};
  int err = WSAStartup(MAKEWORD(2, 0), &wsaData);
  Assert(err == 0, "unable to startup windows sockets");
#endif
}

void Teardown() {
#ifndef EMSCRIPTEN
  WSACleanup();
#endif
}

};