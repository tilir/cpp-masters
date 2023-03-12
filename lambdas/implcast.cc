//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// implicit cast to function pointer
//
//----------------------------------------------------------------------------

#include "gtest/gtest.h"
#include <type_traits>

TEST(lamdas, implcast) {
  auto l = [](int x, int y) -> int { return x + y; };
  int (*pf)(int x, int y) = l;
  EXPECT_EQ(pf(14, 28), 42);
}