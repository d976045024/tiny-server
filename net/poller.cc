#include "net/poller.h"
#include "net/channel.h"
#include "net/poll_poller.h"

using namespace tinyserver;
using namespace tinyserver::net;

Poller::Poller(EventLoop *loop) : owner_loop_(loop) {}

Poller::~Poller() = default;

bool Poller::hasChannel(Channel *channel) const {
  assertInLoopThread();
  auto it = channels_.find(channel->fd());
  return it != channels_.end() && it->second == channel;
}

Poller* Poller::newDefaultPoller(EventLoop *loop) {
  return new PollPoller(loop);
}