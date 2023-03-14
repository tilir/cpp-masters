//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// concept of location: unclear UB case
//
//----------------------------------------------------------------------------

#include <iostream>
#include <thread>

#include "gtest/gtest.h"

namespace {

unsigned x = 0, i = 0, j = 0;

void __attribute__((noinline)) use(int c) { asm(""); }

void readerf() {
  while (i++ < 'g') {
    x += 0x1;
    use(x);
  }
}

void writerf() {
  while (j++ < 'g') {
    x += 0x10000;
    use(x);
  }
}

} // namespace

TEST(threads, locations_ub) {
  std::thread t1{readerf}, t2{writerf};
  t1.join();
  t2.join();
  EXPECT_EQ(x & 0xFF, 'g');
  EXPECT_EQ((x >> 16) & 0xFF, 'g');
}
