//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// variadic pack expansion
//
//----------------------------------------------------------------------------

#include "gtest/gtest.h"
#include <vector>

template <typename... Args> int foo(Args... args) {
  auto lm1 = [args...] { return sizeof...(args); };
  auto lm2 = [... xs = args] { return sizeof...(xs); };
  auto lm3 = [&... xs = args] { return sizeof...(xs); };
  auto lm4 = [... xs = std::move(args)] { return sizeof...(xs); };
  return lm1() + lm2() + lm3() + lm4();
}

TEST(lamdas, vapack) {
  auto result = foo(1, 2.0, std::vector{1, 2, 3});
  EXPECT_EQ(result, 12);
}