#pragma once

#include <map>
#include <vector>

#include "base/timestamp.h"
#include "net/eventloop.h"

struct pollfd;

namespace tinyserver
{

namespace net
{

class Channel;

class Poller {
 public:
  using ChannelList = std::vector<Channel*>;

  Poller(EventLoop* owner_loop);
  virtual ~Poller();

  virtual Timestamp poll(int timeout_ms, ChannelList* active_channels) = 0;

  virtual void updateChannel(Channel *channel) = 0;

  virtual void removeChannel(Channel *channel) = 0;
  
  virtual bool hasChannel(Channel *channel) const;

  static Poller* newDefaultPoller(EventLoop *loop);
  
  void assertInLoopThread() const {
    owner_loop_->assertInLoopThread();
  }

 protected:
  using ChannelMap = std::map<int, Channel*>;
  ChannelMap channels_;

 private:
  EventLoop *owner_loop_;
};

}

}