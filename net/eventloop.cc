#include "net/eventloop.h"
#include "net/channel.h"
#include "base/logger.h"
#include "net/poller.h"

#include <cassert>
#include <poll.h>

using namespace tinyserver;
using namespace tinyserver::net;

namespace
{

__thread EventLoop *t_loopInThisThread = nullptr;

const int kPollTimeMs = 10000;

}

EventLoop::EventLoop()
  : looping_(false),
    quit_(false),
    poller_(Poller::newDefaultPoller(this)),
    thread_id_(CurrentThread::tid()) {
  LOG_INFO("EventLoop created %p in thread %d", this, thread_id_);
  if (t_loopInThisThread) {
    LOG_ERROR("Another EventLoop %p exists in this thread %d", t_loopInThisThread, thread_id_);
  } else {
    t_loopInThisThread = this;
  }
}

EventLoop::~EventLoop() {
  assert(!looping_);
  t_loopInThisThread = nullptr;
}

void EventLoop::loop() {
  assert(!looping_);
  assertInLoopThread();
  looping_ = true;
  quit_ = false;

  while (!quit_) {
    active_channels_.clear();
    poll_return_time_ = poller_->poll(kPollTimeMs, &active_channels_);

    for (auto ch : active_channels_) {
      ch->handleEvent(poll_return_time_);
    }
  }

  LOG_INFO("EventLoop %p stop looping", this);
  looping_ = false;
}

void EventLoop::quit() {
  quit_ = true;
}

EventLoop* EventLoop::getEventLoopOfCurrentThread() {
  return t_loopInThisThread;
}

void EventLoop::abortNotInLoopThread() {
  LOG_ERROR("EventLoop::abortNotInLoopThread - EventLoop %p was created in thread_id_ = %d, current thread id = %d", this, thread_id_, CurrentThread::tid());
}

void EventLoop::updateChannel(Channel* ch) {
  assert(ch->ownerLoop() == this);
  assertInLoopThread();
  poller_->updateChannel(ch);
}