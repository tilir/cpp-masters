//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// Return via reference
//
//----------------------------------------------------------------------------

#include <iostream>
#include <thread>

#include "gtest/gtest.h"

TEST(queue, basic_return) {
  int result = 42;

  auto divi = [](int &result, auto a, auto b) { result = a / b; };

  std::thread t(divi, std::ref(result), 30, 6);
  t.join();
  EXPECT_EQ(result, 5);
}