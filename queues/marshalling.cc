//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// Marshalling of exceptions
//
//----------------------------------------------------------------------------

#include <future>
#include <iostream>
#include <string>
#include <thread>
#include <utility>

#include "gtest/gtest.h"

TEST(queue, marshalling) {
  std::promise<int> p;
  std::future<int> f = p.get_future();

  auto divi = [](auto &&result, auto a, auto b) {
    try {
      if (b == 0)
        throw std::overflow_error("Divide by zero");
      result.set_value(a / b);
    } catch (...) {
      result.set_exception(std::current_exception());
    }
  };

  std::thread t(divi, std::move(p), 30, 0);
  t.detach();

  try {
    auto x = f.get();
    EXPECT_EQ(0, 1); // we shall not be here
  } catch (std::exception &e) {
    EXPECT_EQ(std::string(e.what()), "Divide by zero");
  }
}