#include <gtest/gtest.h>
#include <thread>
#include <iostream>
#include <mutex>

#include "base/current_thread.h"

using namespace std;
using namespace tinyserver;

TEST(CurrentThreadTest, SimpleTest) {
  cout << this_thread::get_id() << endl;
  cout << CurrentThread::tid() << endl;
  vector<thread> threads(10);
  std::mutex mtx;
  for (int i = 0; i < 10; i++) {
    threads[i] = thread([&](int id) {
      mtx.lock();
      cout << "id = " << id << ", thread id = " << CurrentThread::tid() << endl;
      mtx.unlock();
    }, i);
  }
  for (auto &t : threads) {
    t.join();
  }
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}