//-----------------------------------------------------------------------------
//
// Source code for MIPT masters course on C++
// Slides: https://sourceforge.net/projects/cpp-lects-rus
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
//  simple example on ICS tail sequence importance
//
//----------------------------------------------------------------------------

#include "gtest/gtest.h"

namespace {

struct A {
  operator int() { return 42; }
  operator double() { return 1.0; }
};

} // namespace

TEST(functemplates, icstail) {
  int Res = A{};
  EXPECT_EQ(Res, 42);
}