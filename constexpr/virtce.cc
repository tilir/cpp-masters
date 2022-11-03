//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// virtual constexpr example: fail
//
//-----------------------------------------------------------------------------

#include "gtest/gtest.h"
#include <array>

struct Base {
  constexpr Base() = default;
  virtual constexpr int data() const { return 1; }
};

struct Derived : Base {
  constexpr Derived() = default;
  constexpr int data() const override { return 2; }
};

TEST(cexpr, opnew) {
  constexpr Derived d;
  constexpr const Base *pb = &d; // error
  EXPECT_EQ(pb->data(), 2);
}