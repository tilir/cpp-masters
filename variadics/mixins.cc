//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// Examples with mixins
//
//----------------------------------------------------------------------------

#include "gtest/gtest.h"

int g = 0;

struct C1 {
  C1() { g += 1; }
};

struct C2 {
  C2() { g += 2; }
};

template <typename... Mixins> struct mixture : public Mixins... {
  mixture(Mixins... ms) : Mixins(ms)... {};
};

TEST(variadic, mixins) {
  mixture<C1, C2> m(C1{}, C2{});
  mixture<> n;
  EXPECT_EQ(g, 3);
}