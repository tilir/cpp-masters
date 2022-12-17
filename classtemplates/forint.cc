//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// Overload for int
//
//----------------------------------------------------------------------------

#include "gtest/gtest.h"
#include <concepts>

template <typename T> struct Type2Type { using OriginalType = T; };

template <typename T1, typename T2> struct A {
  int func() { return internal_func(Type2Type<T1>()); }

private:
  template <typename V> int internal_func(V) { return 1; }
  int internal_func(Type2Type<int>) { return 2; }
};

TEST(classtempls, forint) {
  A<int, double> a;
  A<float, double> b;
  EXPECT_EQ(a.func(), 2);
  EXPECT_EQ(b.func(), 1);
}