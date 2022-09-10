//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
//  name resolution examples
//
//----------------------------------------------------------------------------

#include "gtest/gtest.h"
#include <concepts>

struct foo {
  int x = 1;
};

TEST(ovrnames, structtest) {
  foo(s);
  EXPECT_EQ(s.x, 1);
}

int foo(...) { return 2; }

TEST(ovrnames, variadics) {
  int s = 0;
  foo(s);
  EXPECT_EQ(foo(s), 2);
}

struct C {
  C(int){};
};

int foo(C x) { return 3; }

TEST(ovrnames, usrtest) {
  int s = 0;
  foo(s);
  EXPECT_EQ(foo(s), 3);
}

int foo(double x) { return 4; }

TEST(ovrnames, impltest) {
  int s = 0;
  foo(s);
  EXPECT_EQ(foo(s), 4);
}

template <typename T> int foo(T x) { return 5; }

TEST(ovrnames, templtest) {
  int s = 0;
  foo(s);
  EXPECT_EQ(foo(s), 5);
}

int foo(int x) { return 6; }

TEST(ovrnames, functest) {
  int s = 0;
  foo(s);
  EXPECT_EQ(foo(s), 6);
}
