//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// Teaser views: for some reasons have problems with clang 15
//
//----------------------------------------------------------------------------

#include "gtest/gtest.h"
#include <iterator>
#include <ranges>
#include <sstream>
#include <string>
#include <vector>

namespace ranges = std::ranges;
namespace views = std::views;

TEST(rngs, teaser) {
  std::vector<int> v;
  std::string s = "1 9 2 4 8 6 3 5 7";
  std::istringstream is{s};
  static auto &os = std::cout;

  // filter: 1 2 4 3
  // transform: 2 4 8 6
  auto t = ranges::istream_view<int>(is) |
           ranges::views::filter([](int n) { return n < 5; }) |
           ranges::views::transform([](int n) { return n * 2; });

  ranges::copy(t, std::back_inserter(v));

  EXPECT_EQ(v[0], 2);
  EXPECT_EQ(v[1], 4);
  EXPECT_EQ(v[2], 8);
  EXPECT_EQ(v[3], 6);
}