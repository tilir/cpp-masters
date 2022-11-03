//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// UB case demo
//
//-----------------------------------------------------------------------------

#include "gtest/gtest.h"

template <typename FwdIt, typename Value>
constexpr FwdIt static_find(FwdIt it, FwdIt fin, Value v) {
  while ((v != *it) && (it != fin))
    ++it;
  return it;
}

TEST(cexpr, ubcase) {
  constexpr int a[] = {1};
  constexpr auto res = static_find(a, a + 1, 4);
  EXPECT_EQ(res, a + 2);
}