//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// Problem of capture list forward: solved but little hacky
//
//----------------------------------------------------------------------------

#include "gtest/gtest.h"
#include <utility>

namespace {

template <typename T> auto fwd_capture(T &&x) {
  struct CapT {
    T value;
  };
  return CapT{std::forward<T>(x)};
}

} // namespace

TEST(lamdas, fwd_capture) {
  auto foo = []<typename T>(T &&a) {
    return [a = fwd_capture(a)]() mutable { return ++a.value; };
  };

  int x = 1;

  auto f1 = foo(1);
  EXPECT_EQ(f1(), 2);

  auto f2 = foo(x);
  EXPECT_EQ(f2(), 2);
  EXPECT_EQ(f2(), 3);
  EXPECT_EQ(f2(), 4);
  EXPECT_EQ(x, 4);
}