//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// core constant expression detailed
//
//-----------------------------------------------------------------------------

#include "gtest/gtest.h"
#include <array>

consteval bool negate(bool x) { return !x; }

template <typename Predicate> constexpr int f(Predicate pred) {
  if constexpr (pred(true))
    return 1;
  return 0;
}

TEST(cexpr, pred) {
  constexpr int x = f(negate);
  EXPECT_EQ(x, false);
}