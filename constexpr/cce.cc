//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// constexpr core constant expression tricky case
//
//-----------------------------------------------------------------------------

#include "gtest/gtest.h"

struct S {
  int n_;
  S(int n) : n_(n) {} // non-constexpr ctor!
  constexpr int get() { return 42; }
};

TEST(cexpr, cce) {
  S s{2};
  constexpr int k = s.get();
  EXPECT_EQ(k, 42);
}