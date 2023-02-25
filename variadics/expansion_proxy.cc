//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// Exapnsion linear seq: mitigating commas
//
//----------------------------------------------------------------------------

#include "gtest/gtest.h"

namespace {

int g = 0;

void bar(int x) { g += x; }

struct expand_t {
  template <typename... T> expand_t(T...) {}
};

template <typename... T> void foo(T... ts) {
  expand_t{(bar(ts), void(), 0)...};
}

} // namespace

TEST(variadic, expansion_proxy) {
  foo(1, 2, 3);
  EXPECT_EQ(g, 6);
}