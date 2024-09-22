#pragma once

#include <cstdint>
#include <thread>

namespace tinyserver
{
namespace CurrentThread
{

  inline pid_t tid()
  {
    return pthread_mach_thread_np(pthread_self());
  }
}
}
