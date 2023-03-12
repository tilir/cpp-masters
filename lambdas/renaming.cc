//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// Capture with renaming
//
//----------------------------------------------------------------------------

#include "gtest/gtest.h"
#include <vector>

TEST(lamdas, renaming_simple) {
  int a = 1;
  auto lmd = [&ra = a, va = a] { return va + ra; };
  int result = lmd();
  EXPECT_EQ(result, 2);
  a += 1;
  result = lmd();
  EXPECT_EQ(result, 3);
}

TEST(lamdas, renaming_move) {
  std::vector<int> a = {1, 2, 3};
  auto lmd = [va = std::move(a)] { return va[0]; };
  int result = lmd();
  EXPECT_EQ(result, 1);
  EXPECT_EQ(a.size(), 0);
}