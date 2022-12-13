//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// Find in transformed view
//
//----------------------------------------------------------------------------

#include "gtest/gtest.h"
#include <ranges>
#include <vector>

namespace ranges = std::ranges;
namespace views = std::views;

struct S {
  int x, y;
};

TEST(rngs, transformed) {
  std::vector<int> v{1, 2, 4};
  auto it1 = ranges::find(v, 4);
  EXPECT_EQ(*it1, 4);

  std::vector<S> w{{1, 0}, {2, 0}, {4, 0}};
  auto it2 = ranges::find_if(w, [](const S &s) { return s.x == 4; });
  EXPECT_EQ(it2->x, 4);

  auto vv = views::transform(w, std::mem_fn(&S::x));
  auto it3 = ranges::find(vv, 4);
  EXPECT_EQ(*it3, 4);
  EXPECT_EQ(it3.base()->x, 4);
}