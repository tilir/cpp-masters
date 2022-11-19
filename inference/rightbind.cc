//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// Rvalue reference binding example
//
//----------------------------------------------------------------------------

#include "gtest/gtest.h"

TEST(rvrefs, binding_r) {
  int x = 1;
  int &a = x;
  a = a + 1;
  int const &c = x + 1;
  int &&d = x + 1;
  d += 1;
  x = a + c + d; // 2 + 3 + 4
  EXPECT_EQ(x, 9);
}