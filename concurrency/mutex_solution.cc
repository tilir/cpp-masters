//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// solution for race with mutex
//
//----------------------------------------------------------------------------

#include <iostream>
#include <thread>
#include <unistd.h>

#include "gtest/gtest.h"

namespace {

volatile int x = 0;
std::mutex mforx;

void __attribute__((noinline)) use(int c) { asm(""); }

void race() {
  // going up
  for (int i = 0; i < 1000000; ++i) {
    mforx.lock();
    x += 1;
    use(x);
    mforx.unlock();
  }

  // going down
  for (int i = 0; i < 1000000; ++i) {
    mforx.lock();
    x -= 1;
    use(x);
    mforx.unlock();
  }
}

} // namespace

TEST(threads, mutex_solution) {
  std::thread t1{race};
  std::thread t2{race};
  t1.join();
  t2.join();
  EXPECT_EQ(x, 0);
}
