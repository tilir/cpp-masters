//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// take more than you can handle
//
//----------------------------------------------------------------------------

#include "gtest/gtest.h"
#include <ranges>
#include <vector>

namespace ranges = std::ranges;
namespace views = std::views;

TEST(rngs, takeit) {
  std::vector<int> v{1, 2, 3};
  auto tv = ranges::take_view(views::all(v), 4);
  EXPECT_EQ(tv[0], 1);
  EXPECT_EQ(tv[1], 2);
  EXPECT_EQ(tv[2], 3);
  EXPECT_EQ(tv.size(), 3);
}
