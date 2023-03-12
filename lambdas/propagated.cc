//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// Propagating constness
//
//----------------------------------------------------------------------------

#include "gtest/gtest.h"
#include <utility>

TEST(lamdas, propagated) {
  int a = 5;
  auto parametrized = [&a](int x) {
    a += x;
    return a;
  };
  auto result = parametrized(9);
  EXPECT_EQ(result, 14);
}

#if (ERROR == 1)

TEST(lamdas, asconst) {
  const int a = 5;
  auto parametrized = [&a](int x) {
    a += x;
    return a;
  };
  auto result = parametrized(9);
  EXPECT_EQ(result, 14);
}

TEST(lamdas, asconst2) {
  int a = 5;
  auto parametrized = [&r = std::as_const(a)](int x) mutable {
    r += x;
    return r;
  };
  auto result = parametrized(9);
  EXPECT_EQ(result, 14);
}

#endif