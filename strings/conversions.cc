//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
//  Example of lvalue to rvalue conversions.
//  Try assemble this and look into side effects.
//
//----------------------------------------------------------------------------

#include <cstddef>
#include <iostream>

#include "gtest/gtest.h"

int foo() {
  volatile int a = 10;
  int b = a;
  return b;
}

int *bar() {
  volatile std::nullptr_t a = nullptr;
  int *b;
  b = a;
  return b;
}

TEST(strings, conversions) {
  auto A = foo();
  EXPECT_EQ(A, 10);

  auto B = bar();
  EXPECT_EQ(B, nullptr);
}