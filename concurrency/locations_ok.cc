//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// concept of location: unclear OK case
//
//----------------------------------------------------------------------------

#include <iostream>
#include <thread>

#include "gtest/gtest.h"

namespace {

void __attribute__((noinline)) use(int c) { asm(""); }

char x[2] = {0, 0}, i = 0, j = 0;

void readerf() {
  while (i++ < 'g') {
    x[0] += 0x1;
    use(x[0]);
  }
}

void writerf() {
  while (j++ < 'g') {
    x[1] += 0x1;
    use(x[1]);
  }
}

} // namespace

TEST(threads, locations_ok) {
  std::thread t1{readerf}, t2{writerf};
  t1.join();
  t2.join();
  EXPECT_EQ(x[0], 'g');
  EXPECT_EQ(x[1], 'g');
}
