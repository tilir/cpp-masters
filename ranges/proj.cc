//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// Projection versus predicate
//
//----------------------------------------------------------------------------

#include "gtest/gtest.h"
#include <algorithm>
#include <ranges>
#include <vector>

namespace ranges = std::ranges;

struct S {
  int x, y;
};

TEST(rngs, sort_comp) {
  std::vector<S> v{{3, 1}, {4, 1}, {5, 9}, {2, 6}};
  ranges::sort(v, [](const auto &a, const auto &b) { return a.x < b.x; });
  EXPECT_EQ(v[0].x, 2);
  EXPECT_EQ(v[1].x, 3);
  EXPECT_EQ(v[2].x, 4);
  EXPECT_EQ(v[3].x, 5);
}

TEST(rngs, sort_proj) {
  std::vector<S> v{{3, 1}, {4, 1}, {5, 9}, {2, 6}};
  ranges::sort(v, {}, &S::x);
  EXPECT_EQ(v[0].x, 2);
  EXPECT_EQ(v[1].x, 3);
  EXPECT_EQ(v[2].x, 4);
  EXPECT_EQ(v[3].x, 5);
}