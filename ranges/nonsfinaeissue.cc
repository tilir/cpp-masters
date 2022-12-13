//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// starnge issue: non-SFINAE repro
//
//----------------------------------------------------------------------------

#include "gtest/gtest.h"
#include <iterator>
#include <string>
#include <vector>

struct S {
  int result() { return 42; }
};

template <typename Inner> struct Outer {
  int result() requires requires { this->inner.result(); }
  { return inner.result(); }
  Inner inner;
};

TEST(rngs, nonsfinae) {
  Outer<S> c;
  int n = c.result();

  EXPECT_EQ(n, 42);
}