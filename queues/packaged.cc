//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// std::packaged task example
//
//----------------------------------------------------------------------------

#include <future>
#include <iostream>
#include <thread>
#include <utility>

#include "gtest/gtest.h"

TEST(queue, packages) {
  auto divi = [](auto a, auto b) {
    if (b == 0)
      throw std::overflow_error("Divide by zero");
    return a / b;
  };

  std::packaged_task<int(int, int)> task{divi};
  std::future<int> f = task.get_future();
#if defined(EXC)
  std::thread t(std::move(task), 30, 0);
#else
  std::thread t(std::move(task), 30, 5);
#endif
  t.detach();

  try {
    auto x = f.get();
    EXPECT_EQ(x, 6);
  } catch (std::exception &e) {
    EXPECT_EQ(std::string(e.what()), "Divide by zero");
  }
}