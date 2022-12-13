//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// transform over filter: direct combination
//
//----------------------------------------------------------------------------

#include "gtest/gtest.h"
#include <ranges>
#include <vector>

namespace views = std::views;

TEST(rngs, simplecomb) {
  std::vector v = {1, 9, 2, 4, 8, 6, 3, 5, 7};
  auto vf = views::filter(v, [](int n) { return n < 5; });
  auto vt = views::transform(vf, [](int n) { return n * 2; });
  EXPECT_EQ(*vt.begin(), 2);
  EXPECT_EQ(*std::next(vt.begin()), 4);
}