//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
//  illustration of point of declaration
//
//----------------------------------------------------------------------------

#include "gtest/gtest.h"
#include <concepts>

TEST(heteromax, ceval) {
  int x = 2;
  {
    int x[x];
    EXPECT_EQ(sizeof(x), 2 * sizeof(int)); // passed
  }

  int y = 2;
  {
    int y = y;
    EXPECT_EQ(y, 2); // failed
  }
}