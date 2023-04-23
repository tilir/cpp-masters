//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// sum fold with and without fwd
//
//----------------------------------------------------------------------------

#include "gtest/gtest.h"

template <typename... Ts> auto sum_all(Ts &&...args) { return (... + args); }

template <typename... Ts> auto sum_all_fwd(Ts &&...args) {
  return (... + std::forward<Ts>(args));
}

TEST(variadic, sumall) {
  int res;
  res = sum_all(1, 2, 3, 4);
  EXPECT_EQ(res, 10);
  res = sum_all_fwd(1, 2, 3, 4);
  EXPECT_EQ(res, 10);
}