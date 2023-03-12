//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// Usage of std invoke
//
//----------------------------------------------------------------------------

#include "gtest/gtest.h"
#include <functional>

namespace {

int foo(int x) { return 1; }

struct S {
  int n = 2;
  int foo(int y) { return 3; }
};

} // namespace

TEST(lamdas, invoke) {
  S s;
  auto psf = &S::foo;
  auto psn = &S::n;

  auto r1 = std::invoke(foo, 1);
  auto r2 = std::invoke(psf, s, 1);
  auto r3 = std::invoke(psf, &s, 1);
  auto r4 = std::invoke(psn, s);

  EXPECT_EQ(r1, 1);
  EXPECT_EQ(r2, 3);
  EXPECT_EQ(r3, 3);
  EXPECT_EQ(r4, 2);
}