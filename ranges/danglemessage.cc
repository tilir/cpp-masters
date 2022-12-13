//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// Dangle where by human intuition shall not
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

template <typename T> T &as_lvalue(T &&t) noexcept { return t; }

TEST(rngs, danglemessage) {
  std::vector<S> v{{1, 0}, {2, 0}, {4, 0}};
  auto it = ranges::find(as_lvalue(v | views::transform(std::mem_fn(&S::x))), 4)
                .base();
  EXPECT_EQ(it->x, 4);
}