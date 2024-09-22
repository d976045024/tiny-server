#pragma once

#include "net/poller.h"

#include <vector>

struct pollfd;

namespace tinyserver
{

namespace net
{

class PollPoller : public Poller {
 public:
  PollPoller(EventLoop *loop);
  ~PollPoller() override;

  Timestamp poll(int timeout_ms, ChannelList *active_channels) override;
  void updateChannel(Channel *channel) override;
  void removeChannel(Channel *channel) override;

 private:
  void fillActiveChannels(int num_events, ChannelList* active_channels) const;

  using PollFdList = std::vector<struct pollfd>;
  PollFdList pollfds_;
};

}

}