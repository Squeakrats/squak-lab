#include "utility.h"
#include <squak/net/Poller.h>

namespace net {

void Poller::Add(SOCKET socket, OnEvent onEvent) {
  Assert(this->sockets.find(socket) == this->sockets.end(),
         "socket is already registered");

  this->sockets.insert(std::make_pair(socket, onEvent));
}

void Poller::Remove(SOCKET socket) {
  Assert(this->sockets.find(socket) != this->sockets.end(),
         "socket isn't registered");
  this->sockets.erase(socket);
}

void Poller::Poll() {
  std::vector<WSAPOLLFD> toPoll{};

  for (auto& entry : this->sockets) {
    toPoll.push_back(WSAPOLLFD{ entry.first, POLLRDNORM });
  }

  if (toPoll.size() == 0) {
    return;
  }

  int count = WSAPoll(toPoll.data(), toPoll.size(), 0);
  Assert(count >= 0, "poll failed");
  if (count == 0) {
    return;
  }

  for (auto& fd : toPoll) {
    if ((fd.revents & POLLNVAL) | (fd.events & POLLHUP)) {
      Panic("poll failed");
    }

    if (fd.revents & POLLRDNORM) {
      this->sockets.at(fd.fd)(EventType::Read);
    }
  }
}

};
