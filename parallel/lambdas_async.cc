//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// Idea of std::async and exception marshalling
//
//----------------------------------------------------------------------------

#include <future>
#include <iostream>
#include <thread>
#include <utility>

#include "gtest/gtest.h"

namespace {
auto divi = [](auto a, auto b) {
  if (b == 0)
    throw std::overflow_error("Divide by zero");
  return a / b;
};
}

TEST(parallel, lambdas_async) {
  std::future<int> f = std::async(divi, 30, 5);
  std::future<int> g = std::async(divi, 30, 0);
  try {
    auto x = f.get();
    EXPECT_EQ(x, 6);
    auto y = g.get();
    EXPECT_EQ(0, 1);
  } catch (std::exception &e) {
    std::string w = e.what();
    EXPECT_EQ(w, "Divide by zero");
  }
}