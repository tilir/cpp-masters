//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// Exapnsion linear seq: init list
//
//----------------------------------------------------------------------------

#include "gtest/gtest.h"

namespace {

int g = 0;

void bar(int x) { g += x; }

template <typename... T> void foo(T... ts) {
  using R = int[sizeof...(ts)];
  (void)R{(bar(ts), 0)...};
}

} // namespace

TEST(variadic, expansion_initlist) {
  foo(1, 2, 3);
  EXPECT_EQ(g, 6);
}