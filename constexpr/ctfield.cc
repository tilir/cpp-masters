//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// Attempt of constexpr field in structure
//
//-----------------------------------------------------------------------------

#include "gtest/gtest.h"
#include <concepts>

struct S {
  constexpr int n = 42;
};

TEST(cexpr, ctfield) {
  S s;
  EXPECT_EQ(s.n, 42);
}