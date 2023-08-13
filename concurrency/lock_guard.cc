//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// example of lock_guard
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
    std::lock_guard<std::mutex> lk{mforx};
    x = x + 1;
    use(x);
  }

  // going down
  for (int i = 0; i < 1000000; ++i) {
    std::lock_guard<std::mutex> lk{mforx};
    x = x - 1;
    use(x);
  }
}

} // namespace

TEST(threads, lock_guard) {
  std::thread t1{race};
  std::thread t2{race};
  t1.join();
  t2.join();
  EXPECT_EQ(x, 0);
}
