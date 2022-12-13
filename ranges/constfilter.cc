//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// const filter is broken
//
//----------------------------------------------------------------------------

#include "gtest/gtest.h"
#include <ranges>
#include <vector>

namespace views = std::views;

TEST(rngs, constfilter) {
  std::vector v = {1, 9, 2, 4, 8, 6, 3, 5, 7};
  const auto vf = views::filter(v, [](int n) { return n < 5; });

  // vf : 1 2 4 3
  EXPECT_EQ(*vf.begin(), 1);
  EXPECT_EQ(*std::next(vf.begin()), 2);
}