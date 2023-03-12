//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// This works in gcc 12 and fails in clang 15.
//
//----------------------------------------------------------------------------

#include "gtest/gtest.h"
#include <utility>

TEST(lamdas, tricky_capture) {
  int a = 5;
  auto parametrized = [&r = std::move(std::as_const(a))](int x) {
    return r + x;
  };
  auto result = parametrized(9);
  EXPECT_EQ(result, 14);
}