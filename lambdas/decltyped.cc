//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// Lambda declaration from decltype (via Doumler)
//
//----------------------------------------------------------------------------

#include <string>
#include "gtest/gtest.h"

TEST(lamdas, decltyped) {
  decltype([](auto x) { return 2 * x; }) twice;
  EXPECT_EQ(twice(2), 4);
}