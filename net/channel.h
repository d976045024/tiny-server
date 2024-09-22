#pragma once

#include "base/timestamp.h"

#include <functional>

namespace tinyserver 
{

namespace net 
{

class EventLoop;

// 每个Channel对象自始至终只负责一个文件描述符fd，但它并不拥有这个fd，也不会在析构的时候关闭fd
// Channel会把不同的事件分发为不同的回调，用户无需继承Channel，只要通过设置回调函数即可
// 用户一般不会直接使用Channel类，而会使用更上层的封装，如TcpConnection
class Channel {
 public:
  using EventCallback = std::function<void()>;
  using ReadEventCallback = std::function<void(Timestamp)>;

  Channel(EventLoop *loop, int fd);

  void handleEvent(Timestamp receive_time);
  void setReadCallback(const ReadEventCallback &cb) { readCallback_ = cb; }
  void setWriteCallback(const EventCallback &cb) { writeCallback_ = cb; }
  void setCloseCallback(const EventCallback &cb) { closeCallback_ = cb; }
  void setErrorCallback(const EventCallback &cb) { errorCallback_ = cb; }

  int fd() const { return fd_; }
  int events() const { return events_; }
  void set_revents(int revt) { revents_ = revt; }
  bool isNoneEvent() const { return events_ == kNoneEvent; }

  void enableReading() { events_ |= kReadEvent; update(); }
  void enableWriting() { events_ |= kWriteEvent; update(); }
  void disableReading() { events_ &= ~kReadEvent; update(); }
  void disableWriting() { events_ &= ~kWriteEvent; update(); }

  int index() const { return index_; }
  void set_index(int idx) { index_ = idx; }

  EventLoop *ownerLoop() { return loop_; }

 private:
  // Channel::update会调用EventLoop::updateChannel, 后者会转而调用Poller::updateChannel
  void update();

  static const int kNoneEvent;
  static const int kReadEvent;
  static const int kWriteEvent;

  EventLoop *loop_;
  const int  fd_;
  int        events_;
  int        revents_;
  int        index_;

  ReadEventCallback readCallback_;
  EventCallback writeCallback_;
  EventCallback closeCallback_;
  EventCallback errorCallback_;
};

} // namespace net

} // namespace tinyserver
