//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
//  Specializing for non type parameters.
//
//----------------------------------------------------------------------------

#include "gtest/gtest.h"

namespace {

struct Pair {
  int x = 1, y = 1;
};

template <int N, int *PN, int &RN, Pair P> int foo() {
  return N + *PN + RN + P.x + P.y;
}

constexpr Pair p;
int x = 1;
int y = 1;

template <> int foo<1, nullptr, x, p>() { return 10 + x + p.x + p.y; }

} // namespace

TEST(functemplates, nttpspec) {
  int res = foo<2, &x, y, p>();
  EXPECT_EQ(res, 2 + 1 + 1 + 1 + 1);

  res = foo<1, nullptr, x, p>();
  EXPECT_EQ(res, 10 + 1 + 1 + 1);
}
