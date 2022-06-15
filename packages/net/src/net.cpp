#include <squak/net.h>
#include "utility.h"
#ifndef EMSCRIPTEN
#include <winsock.h>
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