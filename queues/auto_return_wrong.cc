//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// Trying to generalize
//
//----------------------------------------------------------------------------

#include <iostream>
#include <thread>

#include "gtest/gtest.h"

TEST(queue, auto_return_wrong) {
  int result = 42;

  auto divi = [](auto &result, auto a, auto b) { result = a / b; };

  std::thread t(divi, std::ref(result), 30, 6);
  t.join();
  EXPECT_EQ(result, 5);
}