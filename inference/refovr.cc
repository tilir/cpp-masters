//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// reference overload
//
//----------------------------------------------------------------------------

#include "gtest/gtest.h"

int foo(int &p) { return 1; }
int foo(int &&p) { return 2; }
int foo(const int &p) { return 3; }
int foo(const int &&p) { return 4; }

TEST(rvrefs, refovr) {
  int x = 1;
  const int y = 1;
  int r1 = foo(x);
  int r2 = foo(1);
  int r3 = foo(y);
  int r4 = foo(std::move(y));
  EXPECT_EQ(r1, 1);
  EXPECT_EQ(r2, 2);
  EXPECT_EQ(r3, 3);
  EXPECT_EQ(r4, 4);
}