//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// void pack: sum vs and
//
//----------------------------------------------------------------------------

#include "gtest/gtest.h"

template <typename... Ts> auto sum_all(Ts... args) { return (... + args); }

template <typename... Ts> auto and_all(Ts... args) { return (... && args); }

TEST(variadic, voidpack) {
#if 0
  auto ires = sum_all();
  EXPECT_EQ(ires, 10);
#endif

  // unique case: only &&, || and comma can do this
  bool bres = and_all();
  EXPECT_EQ(bres, true);
}