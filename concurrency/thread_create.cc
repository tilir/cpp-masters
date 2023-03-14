//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// Simplest thread creation
//
//----------------------------------------------------------------------------

#include <iostream>
#include <thread>

#include "gtest/gtest.h"

TEST(threads, thread_create) {
  int j = 0;
  std::thread t([&j] { j += 1; });
  t.join();
  EXPECT_EQ(j, 1);
}
