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

#include <string>
#include "gtest/gtest.h"

struct S {
  int result() { return 42; }
};

template <typename Inner>
struct Outer {
  // Member moved before result(): gcc-15/clang-21 reject referring to
  // not-yet-declared members in a requires-clause (class is incomplete there).
  // The non-SFINAE teaching point is unaffected.
  Inner inner;
  int result()
    requires requires { this->inner.result(); }
  {
    return inner.result();
  }
};

TEST(rngs, nonsfinae) {
  Outer<S> c;
  int n = c.result();

  EXPECT_EQ(n, 42);
}