//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
//  Sutter trick: emulating partial function specialization
//
//----------------------------------------------------------------------------

#include "gtest/gtest.h"
#include <concepts>

template <typename T> struct FImpl {
  static int f(T t) { return 0; }
};

template <typename T> struct FImpl<T *> {
  static int f(T *t) { return 1; }
};

template <typename T> int f(T t) { return FImpl<T>::f(t); }

TEST(classtempls, partialfunc) {
  int x;
  int *px;
  double y;
  double *py;

  EXPECT_EQ(f(x), 0);
  EXPECT_EQ(f(px), 1);
  EXPECT_EQ(f(y), 0);
  EXPECT_EQ(f(py), 1);
}