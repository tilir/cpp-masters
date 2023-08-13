//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// virtual constexpr example: ok
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

constexpr int foo() {
  const Base b;
  const Derived d;
  int sum = 0;
  const Base *bases[] = {&b, &d};
  for (auto pb : bases)
    sum += pb->data();
  return sum;
}

TEST(cexpr, virtceok) {
  constexpr int data = foo(); // ok
  EXPECT_EQ(data, 3);
}