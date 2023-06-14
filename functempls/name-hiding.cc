//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
//  simple name hiding example
//
//----------------------------------------------------------------------------

#include "gtest/gtest.h"

namespace {

struct B {
  int f(int) { return 42; }
};

struct D : B {
  // introduces name
  // using B::f;

  // hides name
  int f(const char *) { return 14; }
};

} // namespace

TEST(functemplates, namehiding) {
  D d;
  int res = d.f(0);
  EXPECT_EQ(res, 14);
}
