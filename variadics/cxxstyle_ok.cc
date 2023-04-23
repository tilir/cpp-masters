//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// C++ style: fixed with forward
//
//----------------------------------------------------------------------------

#include "gtest/gtest.h"

static int sum_all() { return 0; }

template <typename T, typename... Ts> static T sum_all(T &&arg, Ts &&...args) {
  return arg + sum_all(std::forward<Ts>(args)...);
}

TEST(variadic, cxxstyle_ok) {
  int res = sum_all(1, 2, 3, 4);
  EXPECT_EQ(res, 10);
}