//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// C++ style: cryptic error
//
//----------------------------------------------------------------------------

#include "gtest/gtest.h"

int sum_all() { return 0; }

template <typename T, typename... Ts> T sum_all(T &&arg, Ts &&...args) {
  return arg + sum_all(args...);
}

TEST(variadic, cxxstyle_wrong) {
  int res = sum_all(1, 2, 3, 4);
  EXPECT_EQ(res, 10);
}