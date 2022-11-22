//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// Exapnsion linear seq: not working
//
//----------------------------------------------------------------------------

#include "gtest/gtest.h"

int g = 0;

void bar(int x) { g += x; }

template <typename... T> void foo(T... ts) { bar(ts)...; }

TEST(variadic, expansion_wrong) {
  foo(1, 2, 3);
  EXPECT_EQ(g, 6);
}