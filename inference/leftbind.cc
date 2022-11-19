//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// Lvalue reference binding example
//
//----------------------------------------------------------------------------

#include "gtest/gtest.h"

TEST(rvrefs, binding_l) {
  int x = 1;
  int &a = x;
  a = a + 1;
  int const &b = x;
  int const &c = x + 1;
  x = a + b + c; // 2 + 2 + 3
  EXPECT_EQ(x, 7);
}