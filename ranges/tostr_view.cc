//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// to string: simple view
//
//----------------------------------------------------------------------------

#include <ranges>
#include <string>
#include <vector>

#include "gtest/gtest.h"

namespace ranges = std::ranges;
namespace views = std::views;

template <typename R> std::string to_string(R rng) {
  auto v = views::common(rng);
  return std::string(v.begin(), v.end());
}

TEST(rngs, tostr) {
  std::vector v = {'a', 'b', 'c', 'd'};
  auto vf = v | views::filter([](int n) { return n % 2; });
  auto s = to_string(vf);
  EXPECT_EQ(s, "ac");
}