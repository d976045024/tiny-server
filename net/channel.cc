#include "base/logger.h"
#include "net/channel.h"
#include "net/eventloop.h"

#include <poll.h>

using namespace tinyserver;
using namespace tinyserver::net;

const int Channel::kNoneEvent = 0;
const int Channel::kReadEvent = POLLIN | POLLPRI;
const int Channel::kWriteEvent = POLLOUT;

Channel::Channel(EventLoop* loop, int fd) 
    : loop_(loop),
      fd_(fd),
      events_(0),
      revents_(0),
      index_(-1) {
}

void Channel::update() {
  loop_->updateChannel(this);
}

void Channel::handleEvent(Timestamp receive_time) {
  if (revents_ & POLLNVAL) {
    LOG_WARN("Channel::handle_event() POLLNVAL");
  }

  if (revents_ & (POLLERR | POLLNVAL)) {
    if (errorCallback_) errorCallback_();
  }

  #ifndef POLLRDHUP
    const int POLLRDHUP = 0;
  #endif

  if (revents_ & (POLLIN | POLLPRI | POLLRDHUP)) {
    if (readCallback_) readCallback_(receive_time);
  }

  if (revents_ & POLLOUT) {
    if (writeCallback_) writeCallback_();
  }
}