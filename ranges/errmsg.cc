//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// Error message quality
//
//----------------------------------------------------------------------------

#include "gtest/gtest.h"
#include <list>
#include <ranges>

namespace ranges = std::ranges;

TEST(rngs, errmsg) {
  std::list<int> l = {1, 9, 2, 4, 8, 6, 3, 5, 7};

#if 0
  ranges::sort(l);
#else
  std::sort(l.begin(), l.end());
#endif

  EXPECT_EQ(*l.begin(), 1);
  EXPECT_EQ(*std::next(l.begin()), 2);
}