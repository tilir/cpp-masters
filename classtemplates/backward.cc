//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// Partial specialization other way
//
//----------------------------------------------------------------------------

#include "gtest/gtest.h"
#include <concepts>

template <typename T> struct vector {};

template <typename T> struct X { int primary = 1; };

template <typename T> struct X<vector<T>> { int partial = 2; };

TEST(classtempls, backward) {
  X<int> a;
  X<vector<int>> b;
  EXPECT_EQ(a.primary, 1);
  EXPECT_EQ(b.partial, 2);
}