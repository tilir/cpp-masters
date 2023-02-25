//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// Expansion pack task
//
//----------------------------------------------------------------------------

#include "gtest/gtest.h"

namespace {

int f(int x) { return x; }

int f(int x, int y, int z) { return x + y + z; }

int f(int x, int y, int z, int v) { return x + y + z + v; }

template <typename... T> int foo(T... args) {
  return f(f(args...) + f(args)...);
}

template <typename... T> int bar(T... args) { return f(f(args, args...)...); }

} // namespace

TEST(variadic, expansion) {
  int res;
  res = foo(1, 2, 3);
  EXPECT_EQ(res, 24);
  res = bar(1, 2, 3);
  EXPECT_EQ(res, 24);
}