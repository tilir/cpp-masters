//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// Problem of capture list forward: this test will fail
//
//----------------------------------------------------------------------------

#include "gtest/gtest.h"
#include <utility>

TEST(lamdas, fwd_capture_problem) {
  auto foo = []<typename T>(T &&a) {
    return [a = std::forward<T>(a)]() mutable { return ++a; };
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