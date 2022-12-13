//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// Views behavior is not lazy, it is pulling
//
//----------------------------------------------------------------------------

#include "gtest/gtest.h"
#include <functional>
#include <numeric>
#include <ranges>
#include <vector>

namespace ranges = std::ranges;
namespace views = std::views;

struct S {
  int x, y;
};

TEST(rngs, notlazy) {
  std::vector<S> w{{1, 0}, {2, 0}, {4, 0}};
  auto vv = views::transform(w, std::mem_fn(&S::x));
  EXPECT_EQ(*ranges::begin(vv), 1);
  w[0].x = 3;
  EXPECT_EQ(*ranges::begin(vv), 3);
}