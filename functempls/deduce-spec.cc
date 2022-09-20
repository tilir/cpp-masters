//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
//  template specialization type deduction
//
//----------------------------------------------------------------------------

#include "gtest/gtest.h"

// for all
template <typename T>
T foo(T x) { return x; }

// for int
template <>
int foo(int x) { return x + 1; }

TEST(ovrnames, deducetest) {
  double d = 1.5;
  d = foo(d);
  EXPECT_EQ(d, 1.5);
  int i = 2;
  i = foo(i);
  EXPECT_EQ(i, 3);
}