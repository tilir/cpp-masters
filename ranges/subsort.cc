//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// subrange sort
//
//----------------------------------------------------------------------------

#include "gtest/gtest.h"
#include <algorithm>
#include <ranges>
#include <vector>

namespace ranges = std::ranges;

template <auto V> struct EndValue {
  bool operator==(auto pos) const { return *pos == V; }
};

TEST(rngs, subsort) {
  std::vector<int> v{1, 4, 2, 8, 16, 32, 64};
  auto w = ranges::subrange(v.begin() + 1, EndValue<8>{});
  ranges::sort(w);
  EXPECT_EQ(v[1], 2);
  EXPECT_EQ(v[2], 4);
}