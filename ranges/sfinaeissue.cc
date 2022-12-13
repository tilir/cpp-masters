//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// strange issue: deep SFINAE roots
//
//----------------------------------------------------------------------------

#include "gtest/gtest.h"

template <typename D> struct CBase {
  int call_foo() requires requires(D &v) { v.foo(); }
  { return static_cast<D *>(this)->foo(); }
};

struct MyView : public CBase<MyView> {
  int foo() { return 42; }
};

TEST(rngs, sfinae) {
  MyView t;
  int n = t.call_foo();
  EXPECT_EQ(n, 42);
}