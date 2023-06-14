//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
//  what can foo(s) mean
//
//----------------------------------------------------------------------------

#include "gtest/gtest.h"

namespace {

struct foo {
  int x = 1;
  foo() = default;
  static int s;
  foo(int x) { s = x; }
};

int foo::s;

} // namespace

TEST(functemplates, structfoo) {
  foo(s);
  EXPECT_EQ(s.x, 1);
}

TEST(functemplates, structctorfoo) {
  int x = 2;
  delete new foo(x);
  EXPECT_EQ(foo::s, 2);
}

namespace {

int foo(int) { return 3; }

} // namespace

TEST(functemplates, fnfoo) {
  int s = 0;
  foo(s);
  EXPECT_EQ(foo(s), 3);
}