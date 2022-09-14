//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
//  template type deduction with default arguments
//
//----------------------------------------------------------------------------

#include <boost/type_index.hpp>
#include <gtest/gtest.h>

#define CT_ERROR 0

template <typename T = double> double foo(T x = 1.5) { return x; }

TEST(deduce, context) {
  double v0 = foo(2.0);
  EXPECT_EQ(v0, 2.0);
  double v1 = foo<int>();
  EXPECT_EQ(v1, 1);
  double v2 = foo();
  EXPECT_EQ(v2, 1.5);
}