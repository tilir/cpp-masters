//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// ST SFINAE example (primitive types are interesting)
//
//----------------------------------------------------------------------------

#include "gtest/gtest.h"

template <typename T> int foo(T) { return 1; }

#ifdef PRIMITIVE
using S = int;
const int ans = 2;
#else
struct P {};
using S = P;
const int ans = 3;
#endif

template <typename T> int call_foo(T t, S s) { return foo(t) + foo(s); }

int foo(S) { return 2; }

TEST(sfinae, stexample) {
  S x;
  int res = call_foo(x, x);
  EXPECT_EQ(res, ans);
}