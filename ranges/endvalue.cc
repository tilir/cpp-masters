//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// Fancy endvalue: from start to value x
//
//----------------------------------------------------------------------------

#include "gtest/gtest.h"
#include <functional>
#include <numeric>
#include <ranges>
#include <vector>

namespace ranges = std::ranges;
namespace views = std::views;

int accumulate(const ranges::input_range auto &coll) {
  int acc = 0;
  for (auto &elem : coll)
    acc += elem;
  return acc;
}

template <auto V> struct EndValue {
  bool operator==(auto pos) const { return *pos == V; }
};

TEST(rngs, endvalue) {
  std::vector<int> v{1, 2, 4, 8, 16, 32, 64};
  auto w = ranges::subrange(v.begin() + 1, EndValue<8>{});
  auto res = accumulate(w);
  EXPECT_EQ(res, 6);
}