//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// Compound concepts
//
//-----------------------------------------------------------------------------

#include <concepts>
#include <string>

#include "gtest/gtest.h"

namespace {

struct S {
  int data_ = 0;
  using inner = int;
  S &operator++() noexcept {
    ++data_;
    return *this;
  }
};

int operator+(S s, int x) { return x + s.data_; }

template <typename T>
int foo(T x)
  requires requires(T a) {
    // Preserve the layout of the teaching example’s requires expression.
    // clang-format off
  { a + 1 } -> std::convertible_to<typename T::inner>;
    // clang-format on
    requires noexcept(++a);
    requires sizeof(T) == 4;
  }
{
  return ++x + 1;
}

} // namespace

TEST(concepts, compound) {
  S s;
  auto x = foo(s);
  EXPECT_EQ(x, 2);
}
