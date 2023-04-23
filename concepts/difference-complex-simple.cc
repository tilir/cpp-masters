//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// Difference between compile-time evaluation of predicate and syntactic check
//
//-----------------------------------------------------------------------------

#include "gtest/gtest.h"

template <typename T> constexpr int somepred() { return 14; }

template <typename T>
requires(somepred<T>() == 42) bool foo(T &&lhs, T &&rhs) { return lhs < rhs; }

template <typename T>
requires requires(T t) { somepred<T>() == 42; }
bool bar(T &&lhs, T &&rhs) { return lhs < rhs; }

TEST(concepts, difference) {
  bool a, b;
  a = bar(1, 2);
  b = foo(1, 2);
  EXPECT_EQ(a, true);
  EXPECT_EQ(b, true);
}