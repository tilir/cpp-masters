//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
//  Partial specialization basic example
//
//----------------------------------------------------------------------------

#include "gtest/gtest.h"
#include <concepts>

template <typename T, typename U> struct Foo { int primary = 0; };

template <typename T> struct Foo<T, T> { int partial = 1; };

template <typename T> struct Foo<T, int> { int partial = 2; };

template <typename T, typename U> struct Foo<T *, U *> { int partial = 3; };

TEST(partialspec, simplest) {
  Foo<int, float> mif;
  EXPECT_EQ(mif.primary, 0);
  Foo<float, float> mff;
  EXPECT_EQ(mff.partial, 1);
  Foo<float, int> mfi;
  EXPECT_EQ(mfi.partial, 2);
  Foo<int *, float *> mp;
  EXPECT_EQ(mp.partial, 3);
}