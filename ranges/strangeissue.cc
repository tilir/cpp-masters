//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// minimized strange issue for clang 15
//
//----------------------------------------------------------------------------

#include "gtest/gtest.h"
#include <iterator>
#include <ranges>
#include <sstream>
#include <string>
#include <vector>

namespace ranges = std::ranges;

TEST(rngs, strangeissue) {
  std::vector<int> v;
  std::string s = "1 9 2 4 8 6 3 5 7";
  std::istringstream is{s};
  static auto &os = std::cout;

  auto t = ranges::istream_view<int>(is);
  ranges::copy(t, std::back_inserter(v));

  EXPECT_EQ(v[0], 1);
  EXPECT_EQ(v[1], 9);
  EXPECT_EQ(v[2], 2);
  EXPECT_EQ(v[3], 4);
}