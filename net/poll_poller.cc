#include "net/poll_poller.h"
#include "base/logger.h"
#include "net/channel.h"

#include <cassert>
#include <cerrno>
#include <poll.h>

using namespace tinyserver;
using namespace tinyserver::net;

PollPoller::PollPoller(EventLoop* loop) : Poller(loop) {}

PollPoller::~PollPoller() {}

Timestamp PollPoller::poll(int timeout_ms, ChannelList* active_channels) {
    int num_events = ::poll(pollfds_.data(), pollfds_.size(), timeout_ms);
    Timestamp now(Timestamp::now());
    if (num_events > 0) {
      LOG_INFO("%d events happened", num_events);
      fillActiveChannels(num_events, active_channels);
    } else if (num_events == 0) {
      LOG_INFO("nothing happened");
    } else {
      LOG_ERROR("poll error");
    }
    return now;
}

void PollPoller::fillActiveChannels(int num_events, ChannelList* active_channels) const {
  for (auto pfd = pollfds_.begin(); pfd != pollfds_.end() && num_events > 0; ++pfd) {
    if (pfd->revents > 0) {
      --num_events;
      auto ch = channels_.find(pfd->fd);
      assert(ch != channels_.end());
      Channel* channel = ch->second;
      channel->set_revents(pfd->revents);
      active_channels->push_back(channel);
    }
  }
}

// 该函数主要根据channel来维护和更新pollfds数组
void PollPoller::updateChannel(Channel *channel) {
  assertInLoopThread();
  LOG_INFO("fd = %d events = %d", channel->fd(), channel->events());
  if (channel->index() < 0) {
    // a new channel
    assert(channels_.find(channel->fd()) == channels_.end());
    struct pollfd pfd;
    pfd.fd = channel->fd();
    pfd.events = static_cast<short>(channel->events());
    pfd.revents = 0;
    pollfds_.push_back(pfd);
    int idx = static_cast<int>(pollfds_.size() - 1);
    channel->set_index(idx);
    channels_[pfd.fd] = channel;
  } else {
    // update existing one
    assert(channels_.find(channel->fd()) != channels_.end());
    assert(channels_[channel->fd()] == channel);
    int idx = channel->index();
    assert(0 <= idx && idx < static_cast<int>(pollfds_.size()));
    struct pollfd& pfd = pollfds_[idx];
    assert(pfd.fd == channel->fd() || pfd.fd == -channel->fd() - 1);
    pfd.events = static_cast<short>(channel->events());
    pfd.revents = 0;
    if (channel->isNoneEvent()) {
      // ignore this pollfd
      pfd.fd = -channel->fd() - 1;
    }
  }
}

void PollPoller::removeChannel(Channel* channel)
{
  Poller::assertInLoopThread();
  LOG_INFO("fd = %d", channel->fd());
  assert(channels_.find(channel->fd()) != channels_.end());
  assert(channels_[channel->fd()] == channel);
  assert(channel->isNoneEvent());
  int idx = channel->index();
  assert(0 <= idx && idx < static_cast<int>(pollfds_.size()));
  const struct pollfd& pfd = pollfds_[idx]; (void)pfd;
  assert(pfd.fd == -channel->fd()-1 && pfd.events == channel->events());
  size_t n = channels_.erase(channel->fd());
  assert(n == 1); (void)n;
  if (static_cast<size_t>(idx) == pollfds_.size()-1) {
    pollfds_.pop_back();
  }
  else {
    int channel_fd_at_end = pollfds_.back().fd;
    iter_swap(pollfds_.begin()+idx, pollfds_.end()-1);
    if (channel_fd_at_end < 0) {
      channel_fd_at_end = -channel_fd_at_end-1;
    }
    channels_[channel_fd_at_end]->set_index(idx);
    pollfds_.pop_back();
  }
}