//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// Basic future / promise
//
//----------------------------------------------------------------------------

#include <future>
#include <iostream>
#include <thread>
#include <utility>

#include "gtest/gtest.h"

TEST(queue, futures) {
  std::promise<int> p;
  std::future<int> f = p.get_future();

  auto divi = [](auto &&result, auto a, auto b) { result.set_value(a / b); };

  std::thread t(divi, std::move(p), 30, 6);
  t.detach();

  EXPECT_EQ(f.get(), 5);
}
