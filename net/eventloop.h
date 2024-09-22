#pragma once

#include "base/current_thread.h"
#include "base/timestamp.h"

#include <vector>
#include <functional>
#include <memory>

namespace tinyserver 
{

namespace net 
{

class Channel;
class Poller;

class EventLoop {
 public:

  EventLoop();
  ~EventLoop();

  void loop();

  void quit();

  // 该函数由Channel::update转调而来，次函数内部会调用Poller::updateChannel
  void updateChannel(Channel* channel);

  bool isInLoopThread() const { return thread_id_ == CurrentThread::tid(); }

  void assertInLoopThread() {
    if (!isInLoopThread()) {
      abortNotInLoopThread();
    }
  }

  static EventLoop* getEventLoopOfCurrentThread();

 private:
  void abortNotInLoopThread();

  using ChannelList = std::vector<Channel*>;

  bool                    looping_;
  bool                    quit_;
  const pid_t             thread_id_;
  Timestamp               poll_return_time_;
  std::unique_ptr<Poller> poller_;
  ChannelList             active_channels_;
};

}
}