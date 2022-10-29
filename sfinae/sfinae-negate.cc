//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// SFINAE cases versus error-cases
//
//----------------------------------------------------------------------------

#include "gtest/gtest.h"
#include <concepts>

int negate(int i) { return 42; }

template <typename T> typename T::value_type negate(const T &t) {
  typename T::value_type n = -t();
  return n;
}

TEST(sfinae, negate) {
  auto n = negate(2.0);
  EXPECT_EQ(n, 42);
}