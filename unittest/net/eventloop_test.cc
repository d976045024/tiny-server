#include <gtest/gtest.h>
#include <iostream>
#include <thread>

#include "net/eventloop.h"

using namespace std;
using namespace tinyserver;
using namespace tinyserver::net;

void threadFunc() {
  printf("threadFUnc(): pid = %d, tid = %d\n", getpid(), CurrentThread::tid());
  EventLoop loop;
  loop.loop();
}

TEST(EventLoopTest, SimpleTest) {
  printf("main(): pid = %d, tid = %d\n", getpid(), CurrentThread::tid());
  EventLoop loop;
  std::thread t(threadFunc);
  loop.loop();
  t.join();
}

TEST(EventLoopTest, FailedTest) {
  EventLoop loop;
  std::thread t([&]{
    loop.loop();
  });
  t.join();
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}